// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerController.h"

#include "ClientPredictedActor.h"
#include "UI/DMLBaseHUD.h"
#include "DMLCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"

void AGamePlayerController::OnPossess(APawn* InPawn)
{
	// CreateWidgets();
	
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

void AGamePlayerController::OnMatchStarted_Implementation()
{
	if (HUD)
		HUD->BP_OnMatchStarted();
}

void AGamePlayerController::OnMatchEnded_Implementation()
{
	if (HUD)
		HUD->BP_OnMatchEnded();
	if (GetPawn())
		GetPawn()->DisableInput(this);
	SetInputMode(FInputModeUIOnly());
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

	CreateWidgets();

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
			HUD->SetGameState(Cast<ADMLGameState>(GetWorld()->GetGameState()));
		}
	}
	if (!Menu)
	{
		Menu = CreateWidget<UUserWidget>(this, MenuClass, FName("Menu"));
	}
}

uint32 AGamePlayerController::RequestPredictedActorID()
{
	const uint32 NewID = NextPredictedActorID++;
	PredictedActors.Add(FPredictedActorInfo( { NewID }) );
	return NewID;
}

void AGamePlayerController::SetPredictedActor(uint32 ID, AClientPredictedActor* PredictedActor)
{
	if (auto pInfo = PredictedActors.FindByPredicate([ID](const FPredictedActorInfo& Info)
	{
		return Info.ClientActorID == ID;
	}))
	{
		check (!pInfo->PredictedActor.IsValid());
		pInfo->PredictedActor = PredictedActor;

		// If both are valid, link up
		if (pInfo->PredictedActor.IsValid() && pInfo->ReplicatedActor.IsValid())
		{
			pInfo->ReplicatedActor->LinkReplicatedWithPredicted(pInfo->PredictedActor.Get());
		}
	}
}

void AGamePlayerController::SetPredictedActorReplicatedActor(uint32 ID, AClientPredictedActor* ReplicatedActor)
{
	if (auto pInfo = PredictedActors.FindByPredicate([ID](const FPredictedActorInfo& Info)
	{
		return Info.ClientActorID == ID;
	}))
	{
		check (!pInfo->ReplicatedActor.IsValid());
		pInfo->ReplicatedActor = ReplicatedActor;

		// If both are valid, link up
		if (pInfo->PredictedActor.IsValid() && pInfo->ReplicatedActor.IsValid())
		{
			pInfo->ReplicatedActor->LinkReplicatedWithPredicted(pInfo->PredictedActor.Get());
		}
	}
}
