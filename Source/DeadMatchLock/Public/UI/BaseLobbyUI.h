// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseLobbyUI.generated.h"

/**
 * 
 */

class ALobbyPlayerController;

UCLASS()
class DEADMATCHLOCK_API UBaseLobbyUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetPlayerController(ALobbyPlayerController* InPlayerController);

	UFUNCTION(BlueprintImplementableEvent)
	void SetAllReady(bool bAllReady);

private:
	UPROPERTY()
	ALobbyPlayerController* PlayerController;

	UFUNCTION(BlueprintCallable)
	void ToggleReadyState();

	UFUNCTION(BlueprintCallable)
	void LeaveLobby();
};
