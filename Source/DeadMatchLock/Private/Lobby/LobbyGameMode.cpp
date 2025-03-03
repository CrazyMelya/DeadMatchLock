// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyGameMode.h"

#include "DMLPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyPlayerController.h"


ALobbyGameMode::ALobbyGameMode(const FObjectInitializer& ObjectInitializer)
{
	PlayerStateClass = ADMLPlayerState::StaticClass();
}

void ALobbyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (auto LobbyPlayerController = Cast<ALobbyPlayerController>(NewPlayer))
	{
		Players.Add(LobbyPlayerController);
		LobbyPlayerController->SetGameMode(this);
		if (bPlatformsSet)
			SetupNewPlayer(LobbyPlayerController);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (auto LobbyPlayerController = Cast<ALobbyPlayerController>(Exiting))
	{
		LobbyPlayerController->RemovePlayerPlatform();
		Players.Remove(LobbyPlayerController);
	}
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetupPlatforms();
	SetupFirstPlayers();
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
