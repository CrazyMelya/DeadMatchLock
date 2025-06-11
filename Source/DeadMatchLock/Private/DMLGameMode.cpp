// Copyright Epic Games, Inc. All Rights Reserved.

#include "DMLGameMode.h"

#include "DMLCharacter.h"
#include "DMLGameInstance.h"
#include "DMLGameState.h"
#include "GamePlayerController.h"
#include "DMLPlayerState.h"
#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerStart.h"
#include "Libraries/DMLFunctionLibrary.h"
#include "UObject/ConstructorHelpers.h"

ADMLGameMode::ADMLGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	PlayerControllerClass = AGamePlayerController::StaticClass();
	PlayerStateClass = ADMLPlayerState::StaticClass();
	GameStateClass = ADMLGameState::StaticClass();
	bDelayedStart = true;
}

void ADMLGameMode::NotifyPlayerDeath(AGamePlayerController* DeadPlayer, AActor* Killer)
{
	if (DeadPlayer)
	{
		if (auto PlayerState = Cast<ADMLPlayerState>(DeadPlayer->PlayerState))
		{
			PlayerState->SetDeaths(PlayerState->Deaths + 1);
		}
		FTimerHandle RespawnTimer;
		GetWorld()->GetTimerManager().SetTimer(
			RespawnTimer,
			FTimerDelegate::CreateUObject(this, &ThisClass::RespawnPlayer, DeadPlayer),
			5.0f,
			false);
		RespawnTimers.Add(DeadPlayer, RespawnTimer);
	}
	if (Killer)
	{
		if (auto KillerController = Cast<AGamePlayerController>(Killer->GetOwner()))
		{
			if (auto PlayerState = Cast<ADMLPlayerState>(KillerController->PlayerState))
			{
				PlayerState->SetKills(PlayerState->Kills + 1);
				if (PlayerState->Kills >= KillsToWin)
					EndMatch();
			}
		}
	}
}

void ADMLGameMode::RespawnPlayer(AGamePlayerController* Controller)
{
	if (Controller)
	{
		RespawnTimers.Remove(Controller);
		if (auto PlayerStart = ChoosePlayerStart(Controller))
		{
			FVector RespawnLocation = PlayerStart->GetActorLocation();
			FRotator RespawnRotation = PlayerStart->GetActorRotation();
			Controller->ClientSetRotation(RespawnRotation);
			TSubclassOf<APawn> PawnClass = DefaultPawnClass;
			auto PlayerState = Controller->GetPlayerState<ADMLPlayerState>();
			if (Characters && PlayerState)
			{
				if (auto CharacterData = Characters->FindRow<FCharacterData>(PlayerState->CharacterName, TEXT("Spawn Player")))
					PawnClass = CharacterData->CharacterClass;
			}
			auto NewCharacter = GetWorld()->SpawnActor<ADMLCharacter>(
				PawnClass,
				RespawnLocation,
				RespawnRotation);
			if (NewCharacter)
			{
				Controller->Possess(NewCharacter);
				Controller->SetControlRotation(RespawnRotation);
				UE_LOG(LogTemp, Log, TEXT("Player respawned at PlayerStart."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No PlayerStart found!"));
		}
	}
}

AActor* ADMLGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> PlayerStarts;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		PlayerStarts.Add(*It);
	}

	if (PlayerStarts.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);
		return PlayerStarts[RandomIndex];
	}
	
	return Super::ChoosePlayerStart_Implementation(Player);
}

APawn* ADMLGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	if (!NewPlayer) return nullptr;

	TSubclassOf<APawn> PawnClass = DefaultPawnClass;
	auto PlayerState = NewPlayer->GetPlayerState<ADMLPlayerState>();
	if (Characters && PlayerState)
	{
		if (auto CharacterData = Characters->FindRow<FCharacterData>(PlayerState->CharacterName, TEXT("Spawn Player")))
			PawnClass = CharacterData->CharacterClass;
	}
	return GetWorld()->SpawnActor<ACharacter>(
		PawnClass,
		StartSpot->GetActorLocation(),
		StartSpot->GetActorRotation()
	);
}

void ADMLGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	if (auto GameInstance = UDMLFunctionLibrary::GetDMLGameInstance(this))
	{
		if (NumPlayers >= GameInstance->GetNumPlayers())
		{
			DMLGameState->GameTime = GameStartTime;
			GetWorld()->GetTimerManager().SetTimer(GameTimer, this, &ThisClass::StartMatchTimerTick, 1.0f, true);
		}
	}
}

void ADMLGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);
}

void ADMLGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (auto GameInstance = UDMLFunctionLibrary::GetDMLGameInstance(this))
	{
		KillsToWin = GameInstance->GetNumKills();
	}
}

void ADMLGameMode::InitGameState()
{
	Super::InitGameState();

	DMLGameState = Cast<ADMLGameState>(GameState);
}

void ADMLGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	GetWorld()->GetTimerManager().ClearTimer(GameTimer);
	DMLGameState->GameTime = 20;
	GetWorld()->GetTimerManager().SetTimer(GameTimer, this, &ThisClass::LeavingMapTimerTick, 1.0f, true);
}

void ADMLGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	DMLGameState->GameTime = 0;
	GetWorld()->GetTimerManager().SetTimer(GameTimer, this, &ThisClass::GameTimerTick, 1.0f, true);
}

void ADMLGameMode::StartMatchTimerTick()
{
	DMLGameState->GameTime--;
	if (DMLGameState->GameTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(GameTimer);
		StartMatch();
	}
}

void ADMLGameMode::GameTimerTick() const
{
	DMLGameState->GameTime++;
}

void ADMLGameMode::LeavingMapTimerTick() const
{
	DMLGameState->GameTime--;
	if (DMLGameState->GameTime <= 0)
	{
		GetWorld()->ServerTravel("/Game/Maps/Lobby", TRAVEL_Absolute);
	}
}
