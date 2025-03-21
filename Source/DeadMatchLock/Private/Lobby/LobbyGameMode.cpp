// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyGameMode.h"

#include "DMLGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyGameState.h"
#include "Lobby/LobbyPlayerController.h"
#include "Lobby/LobbyPlayerState.h"


ALobbyGameMode::ALobbyGameMode(const FObjectInitializer& ObjectInitializer)
{
	PlayerStateClass = ALobbyPlayerState::StaticClass();
	GameStateClass = ALobbyGameState::StaticClass();
}

void ALobbyGameMode::CloseLobby()
{
	for (auto Player : Players)
	{
		if (!Player->IsLocalController())
			Player->LeaveLobby();
	}
}

void ALobbyGameMode::PickCharacter(ALobbyPlayerState* PickingPlayer, const FName& CharacterName)
{
	if (PickingPlayer && AvailableCharacters.Contains(CharacterName))
	{
		AvailableCharacters.Remove(CharacterName);
		LobbyGameState->SetAvailableCharacters(AvailableCharacters);
		PickingPlayer->SetCharacterName(CharacterName);
	}
}

void ALobbyGameMode::PickRandomCharacter(ALobbyPlayerState* PickingPlayer)
{
	auto RandomCharacter = AvailableCharacters[FMath::RandRange(0, AvailableCharacters.Num() - 1)];
	PickCharacter(PickingPlayer, RandomCharacter);
}

void ALobbyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	// OnNewPlayerConnected(Cast<ALobbyPlayerController>(NewPlayer));
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (auto LobbyPlayerController = Cast<ALobbyPlayerController>(Exiting))
	{
		LobbyPlayerController->RemovePlayerPlatform();
		Players.Remove(LobbyPlayerController);
		for (auto Player : Players)
		{
			Player->LobbyPlayerState->SetReadyState(false);
		}
	}
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetupCharacters();
	SetupPlatforms();
	SetupFirstPlayers();
}

void ALobbyGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);

	// OnNewPlayerConnected(Cast<ALobbyPlayerController>(C));
}

void ALobbyGameMode::InitGameState()
{
	Super::InitGameState();

	LobbyGameState = Cast<ALobbyGameState>(GameState);
}

void ALobbyGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	OnNewPlayerConnected(Cast<ALobbyPlayerController>(NewPlayer));
}

void ALobbyGameMode::SetupFirstPlayers()
{
	for (int i = 0; i < Players.Num(); i++)
	{
		Players[i]->SetPlayerPlatform(Platforms[i]);
	}
}

void ALobbyGameMode::SetupPlatforms()
{
	TArray<AActor*> TempActors;
	UGameplayStatics::GetAllActorsOfClass(this, ALobbyPlayerPlatform::StaticClass(), TempActors);

	for (auto Actor : TempActors)
	{
		Platforms.Add(Cast<ALobbyPlayerPlatform>(Actor));
	}

	Platforms.Sort([](const ALobbyPlayerPlatform& A, const ALobbyPlayerPlatform& B)
	{
		if (!A.Tags.IsValidIndex(0) || !B.Tags.IsValidIndex(0))
			return false;
		return A.Tags[0].ToString() < B.Tags[0].ToString(); 
	});

	bPlatformsSet = true;
}

void ALobbyGameMode::SetupCharacters()
{
	if (CharactersDataTable)
	{
		AvailableCharacters = CharactersDataTable->GetRowNames();
		LobbyGameState->SetAvailableCharacters(AvailableCharacters);
	}
}

void ALobbyGameMode::SetupNewPlayer(ALobbyPlayerController* NewPlayer)
{
	for (auto Platform : Platforms)
	{
		if (!Platform->GetPossessed())
		{
			NewPlayer->SetPlayerPlatform(Platform);
			break;
		}
	}
}

void ALobbyGameMode::OnNewPlayerConnected(ALobbyPlayerController* NewPlayer)
{
	if (NewPlayer)
	{
		Players.Add(NewPlayer);
		NewPlayer->SetGameMode(this);
		if (bPlatformsSet)
			SetupNewPlayer(NewPlayer);
		for (auto Player : Players)
		{
			if (Player->LobbyPlayerState)
				Player->LobbyPlayerState->SetReadyState(false);
		}
	}
}

void ALobbyGameMode::StartSelectionStage()
{
	LobbyGameState->SetLobbyStage(Selection);
	LobbyGameState->RemainingTime = SelectionTime;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ALobbyGameMode::SelectCharacterTimerTick, 1.0f, true);
}

void ALobbyGameMode::SelectCharacterTimerTick()
{
	LobbyGameState->RemainingTime--;
	if (LobbyGameState->RemainingTime == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer);
		EndSelectionStage();
	}
}

void ALobbyGameMode::EndSelectionStage()
{
	if (auto GameInstance = Cast<UDMLGameInstance>(GetWorld()->GetGameInstance()))
	{
		GameInstance->SetPlayersNum(LobbyGameState->PlayerArray.Num());
	}
	PickRandomCharacters();
	LobbyGameState->SetLobbyStage(FinalCountdown);
	LobbyGameState->RemainingTime = 10;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ALobbyGameMode::FinalCountdownTick, 1.0f, true);
	
}

void ALobbyGameMode::PickRandomCharacters()
{
	for (auto Player : Players)
	{
		if (!Player->LobbyPlayerState->CharacterSelected())
			PickRandomCharacter(Player->LobbyPlayerState);
	}
}

void ALobbyGameMode::FinalCountdownTick()
{
	LobbyGameState->RemainingTime--;
	if (LobbyGameState->RemainingTime == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer);
		BP_StartGame();
	}
}

