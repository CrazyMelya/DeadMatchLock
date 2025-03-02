// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DMLPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API ADMLPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby State")
	bool bReady = false;

	UFUNCTION(NetMulticast, Reliable)
	void ToggleReadyState();
};
