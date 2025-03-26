// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DMLCharacter.h"
#include "InputAction.h"
#include "UI/DMLBaseHUD.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UDMLBaseHUD* HUD;

	UPROPERTY()
	UUserWidget* Menu;

	UFUNCTION(Client, Reliable)
	void OnMatchStarted();

	UFUNCTION(Client, Reliable)
	void OnMatchEnded();

	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Interface")
	TSubclassOf<UDMLBaseHUD> HUDClass = UDMLBaseHUD::StaticClass();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Interface")
	TSubclassOf<UUserWidget> MenuClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC_Gameplay;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_ToggleMenu;

	UFUNCTION(Client, Reliable)
	void CreateWidgets();

	UFUNCTION(Client, Reliable)
	void UpdateTargetCharacterToHUD();

	UFUNCTION()
	void ToggleMenu();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnRep_Pawn() override;
};
