// Copyright Epic Games, Inc. All Rights Reserved.

#include "DMLGameMode.h"

#include "DMLCharacter.h"
#include "GamePlayerController.h"
#include "DMLPlayerState.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
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
}

void ADMLGameMode::NotifyPlayerDeath(AGamePlayerController* Controller)
{
	if (Controller)
	{
		FTimerHandle RespawnTimer;
		GetWorld()->GetTimerManager().SetTimer(
			RespawnTimer,
			FTimerDelegate::CreateUObject(this, &ADMLGameMode::RespawnPlayer, Controller),
			5.0f,
			false);
		RespawnTimers.Add(Controller, RespawnTimer);
	}
}

void ADMLGameMode::RespawnPlayer(AGamePlayerController* Controller)
{
	if (Controller)
	{
		RespawnTimers.Remove(Controller);
		auto PlayerStart = ChoosePlayerStart(Controller);
		if (PlayerStart)
		{
			FVector RespawnLocation = PlayerStart->GetActorLocation();
			FRotator RespawnRotation = PlayerStart->GetActorRotation();
			auto CharacterClass =  Controller->GetPlayerState<ADMLPlayerState>()->CharacterData.CharacterClass;
			auto NewCharacter = GetWorld()->SpawnActor<ADMLCharacter>(
				CharacterClass ? CharacterClass : DefaultPawnClass,
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

	// Если точек спавна нет, используем стандартное поведение
	return Super::ChoosePlayerStart_Implementation(Player);
}

APawn* ADMLGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	if (!NewPlayer) return nullptr;

	auto PS = NewPlayer->GetPlayerState<ADMLPlayerState>();
	if (PS)
	{
		auto CharacterClass =  PS->CharacterData.CharacterClass;
		return GetWorld()->SpawnActor<ACharacter>(
			CharacterClass ? CharacterClass : DefaultPawnClass,
			StartSpot->GetActorLocation(),
			StartSpot->GetActorRotation()
		);
	}

	// Если нет выбранного класса, спавним дефолтного
	return Super::SpawnDefaultPawnFor(NewPlayer, StartSpot);
}
