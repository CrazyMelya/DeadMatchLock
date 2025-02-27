// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerController.h"


void ALobbyPlayerController::SetPlayerPlatform(ALobbyPlayerPlatform* InPlayerPlatform)
{
	PlayerPlatform = InPlayerPlatform;
}

void ALobbyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsLocalController() && !LobbyUI)
	{
		LobbyUI = CreateWidget<UBaseLobbyUI>(this, LobbyUIClass, FName("LobbyUI"));
		if (LobbyUI)
		{
			LobbyUI->SetPlayerController(this);
			LobbyUI->AddToViewport();
		}
	}
}
