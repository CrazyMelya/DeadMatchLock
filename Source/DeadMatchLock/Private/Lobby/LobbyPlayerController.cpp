// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerController.h"

#include "Online.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


void ALobbyPlayerController::InitPlayerState()
{
	Super::InitPlayerState();

	DMLPlayerState = Cast<ADMLPlayerState>(PlayerState);
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, DMLPlayerState);
	DOREPLIFETIME(ALobbyPlayerController, PlayerPlatform);
}

void ALobbyPlayerController::SetPlayerPlatform(ALobbyPlayerPlatform* InPlayerPlatform)
{
	PlayerPlatform = InPlayerPlatform;
	PlayerPlatform->SetPlayer(DMLPlayerState);
}

void ALobbyPlayerController::RemovePlayerPlatform()
{
	PlayerPlatform->RemovePlayer();
	PlayerPlatform = nullptr;
}

void ALobbyPlayerController::LeaveLobby_Implementation()
{
	if (HasAuthority())
	{
		GameMode->CloseLobby();
		BP_LeaveLobby();
	}
	else
	{
		BP_LeaveLobby();
	}
}

void ALobbyPlayerController::BeginPlay()
{
	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);
	
	if (IsLocalController() && !LobbyUI)
	{
		LobbyUI = CreateWidget<UBaseLobbyUI>(this, LobbyUIClass, FName("LobbyUI"));
		if (LobbyUI)
		{
			LobbyUI->SetPlayerController(this);
			LobbyUI->AddToViewport();
		}
	}

	Super::BeginPlay();
}

void ALobbyPlayerController::SetGameMode(ALobbyGameMode* InGameMode)
{
	GameMode = InGameMode;
}

void ALobbyPlayerController::SetAllReady(bool bAllReady)
{
	LobbyUI->SetAllReady(bAllReady);
}


void ALobbyPlayerController::ToggleReadyState_Implementation()
{
	if (DMLPlayerState && PlayerPlatform)
	{
		DMLPlayerState->ToggleReadyState();
		PlayerPlatform->RefreshInfo();
		GameMode->CheckAllReady();
	}
}
