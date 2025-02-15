// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DMLPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "DMLMainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API ADMLMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

	ADMLMainMenuGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void GetSeamlessTravelActorList(bool bToTransition, TArray<AActor*>& ActorList) override;

	virtual void BeginPlay() override;
};
