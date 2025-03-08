// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyGameMode.h"

#include "DMLPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyPlayerController.h"


ALobbyGameMode::ALobbyGameMode(const FObjectInitializer& ObjectInitializer)
{
	PlayerStateClass = ADMLPlayerState::StaticClass();
}

void ALobbyGameMode::CloseLobby()
{
	for (auto Player : Players)
	{
		if (Player != LobbyLeader)
		{
			Player->LeaveLobby();
		}
	}
}

void ALobbyGameMode::CheckAllReady_Implementation()
{
	for (auto Player : Players)
	{
		if (!Player->DMLPlayerState->bReady)
		{
			LobbyLeader->SetAllReady(false);
			return;
		}
	}
	LobbyLeader->SetAllReady(true);
}

void ALobbyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	OnNewPlayerConnected(Cast<ALobbyPlayerController>(NewPlayer));
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (auto LobbyPlayerController = Cast<ALobbyPlayerController>(Exiting))
	{
		LobbyPlayerController->RemovePlayerPlatform();
		Players.Remove(LobbyPlayerController);
		CheckAllReady();
	}
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetupPlatforms();
	SetupFirstPlayers();
}

void ALobbyGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);

	OnNewPlayerConnected(Cast<ALobbyPlayerController>(C));
}

void ALobbyGameMode::SetupFirstPlayers()
{
	for (int i = 0; i < Players.Num(); i++)
		Players[i]->SetPlayerPlatform(Platforms[i]);
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

void ALobbyGameMode::SetupNewPlayer(ALobbyPlayerController* Player)
{
	for (auto Platform : Platforms)
	{
		if (!Platform->GetPossessed())
		{
			Player->SetPlayerPlatform(Platform);
			break;
		}
	}
}

void ALobbyGameMode::OnNewPlayerConnected(ALobbyPlayerController* Player)
{
	if (Player)
	{
		if (!LobbyLeader && Player->IsLocalController())
			LobbyLeader = Player;
		else
			LobbyLeader->SetAllReady(false);
		Players.Add(Player);
		Player->SetGameMode(this);
		if (bPlatformsSet)
			SetupNewPlayer(Player);
	}
}

