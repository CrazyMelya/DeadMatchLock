// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DMLBaseMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UDMLBaseMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnLobbyUpdated(const FLobbyData& Lobby);
};
