// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerController.h"

#include "UI/DMLBaseHUD.h"
#include "DMLCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"

void AGamePlayerController::OnPossess(APawn* InPawn)
{
	CreateWidgets();
	
	Super::OnPossess(InPawn);

	if (IsLocalController())
		UpdateTargetCharacterToHUD();
}

void AGamePlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	if (IsLocalController())
		UpdateTargetCharacterToHUD();
}

void AGamePlayerController::RefreshGameStats_Implementation()
{
	if (HUD)
		HUD->BP_RefreshGameStats();
}

void AGamePlayerController::ToggleMenu()
{
	if (!Menu) return;

	if (Menu->IsInViewport())
	{
		Menu->RemoveFromParent();
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		Menu->AddToViewport();
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
	}
}

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PC = Cast<APlayerController>(this))
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Gameplay, 0);
		}
	}
	SetInputMode(FInputModeGameOnly());
}

void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (auto EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->BindAction(IA_ToggleMenu, ETriggerEvent::Triggered, this, &AGamePlayerController::ToggleMenu);
	}
}

void AGamePlayerController::UpdateTargetCharacterToHUD_Implementation()
{
	if (!HUD) return;
	
	auto InCharacter = GetPawn<ADMLCharacter>();
	HUD->SetTargetCharacter(InCharacter);
}

void AGamePlayerController::CreateWidgets_Implementation()
{
	if (!HUD)
	{
		HUD = CreateWidget<UDMLBaseHUD>(this, HUDClass, FName("HUD"));
		if (HUD)
		{
			HUD->AddToViewport();
		}
	}
	if (!Menu)
	{
		Menu = CreateWidget<UUserWidget>(this, MenuClass, FName("Menu"));
	}
}
