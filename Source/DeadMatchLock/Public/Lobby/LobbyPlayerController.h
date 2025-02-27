// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyPlayerPlatform.h"
#include "GameFramework/PlayerController.h"
#include "UI/BaseLobbyUI.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	UPROPERTY()
	ALobbyPlayerPlatform* PlayerPlatform;

protected:
	UPROPERTY(BlueprintReadOnly)
	UBaseLobbyUI* LobbyUI;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UBaseLobbyUI> LobbyUIClass;
	
public:
	void SetPlayerPlatform(ALobbyPlayerPlatform* InPlayerPlatform);

	virtual void OnPossess(APawn* InPawn) override;
};
