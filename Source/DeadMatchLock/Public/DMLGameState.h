// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DMLGameState.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API ADMLGameState : public AGameState
{
	GENERATED_BODY()
	
	
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	
};
