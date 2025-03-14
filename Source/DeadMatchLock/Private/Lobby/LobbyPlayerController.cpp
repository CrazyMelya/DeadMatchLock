// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerController.h"

#include "Lobby/LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"


void ALobbyPlayerController::InitPlayerState()
{
	Super::InitPlayerState();

	LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState);
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, LobbyPlayerState);
	DOREPLIFETIME(ThisClass, PlayerPlatform);
}

void ALobbyPlayerController::SetPlayerPlatform(ALobbyPlayerPlatform* InPlayerPlatform)
{
	PlayerPlatform = InPlayerPlatform;
	PlayerPlatform->SetPlayer(LobbyPlayerState);
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
		LobbyGameMode->CloseLobby();
		BP_LeaveLobby();
	}
	else
	{
		ClientTravel("/Game/Maps/MainMenu", TRAVEL_Absolute);
		// BP_LeaveLobby();
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
			LobbyUI->OnCharacterSelected.AddDynamic(this, &ALobbyPlayerController::PickCharacter);
			LobbyUI->AddToViewport();
			LobbyUI->SetGameState(GetWorld()->GetGameState<ALobbyGameState>());
			if (LobbyPlayerState)
				LobbyUI->BindOnCharacterSelected(LobbyPlayerState);
		}
	}
	Super::BeginPlay();
}

void ALobbyPlayerController::SetGameMode(ALobbyGameMode* InGameMode)
{
	LobbyGameMode = InGameMode;
}

void ALobbyPlayerController::OnRep_LobbyPlayerState()
{
	if (LobbyPlayerState && LobbyUI)
		LobbyUI->BindOnCharacterSelected(LobbyPlayerState);
}

void ALobbyPlayerController::StartGame_Implementation()
{
	LobbyGameMode->StartSelectionStage();
}

void ALobbyPlayerController::PickCharacter_Implementation(const FName& CharacterName)
{
	LobbyGameMode->PickCharacter(LobbyPlayerState, CharacterName);
}

void ALobbyPlayerController::ToggleReadyState_Implementation()
{
	if (LobbyPlayerState && PlayerPlatform)
	{
		LobbyPlayerState->ToggleReadyState();
	}
}
