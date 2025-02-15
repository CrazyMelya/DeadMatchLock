// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DMLBaseHUD.h"
#include "GameFramework/PlayerController.h"
#include "DMLPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API ADMLPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UDMLBaseHUD> HUDClass = UDMLBaseHUD::StaticClass();

	UPROPERTY()
	UDMLBaseHUD* HUD;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_Pawn() override;

protected:
	UFUNCTION(Client, Reliable)
	void CreateHUD();
	
	UFUNCTION(Client, Reliable)
	void UpdateTargetCharacterToHUD();
};
