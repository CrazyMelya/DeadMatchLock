// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/DMLBaseMainMenu.h"
#include "DMLMenuPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class DEADMATCHLOCK_API ADMLMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADMLMenuPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UDMLBaseMainMenu> MainMenuClass;

	UPROPERTY(BlueprintReadWrite, Category = "Widget")
	UDMLBaseMainMenu* MainMenuWidget;
};

