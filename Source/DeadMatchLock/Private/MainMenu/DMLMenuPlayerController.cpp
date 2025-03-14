// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/DMLMenuPlayerController.h"

#include "DMLPlayerState.h"

ADMLMenuPlayerController::ADMLMenuPlayerController(const FObjectInitializer& ObjectInitializer)
{
	SetShowMouseCursor(true);
}

void ADMLMenuPlayerController::BeginPlay()
{
	SetInputMode(FInputModeUIOnly());

	if (IsLocalController())
	{
		MainMenuWidget = CreateWidget<UDMLBaseMainMenu>(this, MainMenuClass, FName("MainMenu"));
		if (MainMenuWidget)
			MainMenuWidget->AddToViewport();
	}
	Super::BeginPlay();
}