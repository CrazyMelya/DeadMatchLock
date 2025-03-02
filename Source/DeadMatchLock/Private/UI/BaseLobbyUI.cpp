// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseLobbyUI.h"

#include "Lobby/LobbyPlayerController.h"


void UBaseLobbyUI::ToggleReadyState()
{
	PlayerController->ToggleReadyState();
}

void UBaseLobbyUI::LeaveLobby()
{
	PlayerController->LeaveLobby();
}

void UBaseLobbyUI::SetPlayerController(ALobbyPlayerController* InPlayerController)
{
	PlayerController = InPlayerController;
}
