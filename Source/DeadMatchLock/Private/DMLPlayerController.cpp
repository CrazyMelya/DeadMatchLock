// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLPlayerController.h"

#include "UI/DMLBaseHUD.h"
#include "DMLCharacter.h"
#include "Blueprint/UserWidget.h"

void ADMLPlayerController::OnPossess(APawn* InPawn)
{
	CreateHUD();
	
	Super::OnPossess(InPawn);

	if (IsLocalController())
		UpdateTargetCharacterToHUD();
}

void ADMLPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	if (IsLocalController())
		UpdateTargetCharacterToHUD();
}

void ADMLPlayerController::UpdateTargetCharacterToHUD_Implementation()
{
	if (!HUD) return;
	
	auto InCharacter = GetPawn<ADMLCharacter>();
	HUD->SetTargetCharacter(InCharacter);
}

void ADMLPlayerController::CreateHUD_Implementation()
{
	if (HUD) return;

	HUD = CreateWidget<UDMLBaseHUD>(this, HUDClass, FName("HUD"));
	if (HUD)
		HUD->AddToViewport();
}
