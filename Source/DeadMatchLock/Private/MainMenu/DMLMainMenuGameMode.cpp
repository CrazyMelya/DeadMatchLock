// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/DMLMainMenuGameMode.h"

#include "MainMenu/DMLMenuPlayerController.h"
#include "DMLPlayerState.h"
#include "GameFramework/GameStateBase.h"


ADMLMainMenuGameMode::ADMLMainMenuGameMode(const FObjectInitializer& ObjectInitializer)
{
	PlayerControllerClass = ADMLMenuPlayerController::StaticClass();
	PlayerStateClass = ADMLPlayerState::StaticClass();
}




