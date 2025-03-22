// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DMLGameState.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameTimeChanged, float)

UCLASS()
class DEADMATCHLOCK_API ADMLGameState : public AGameState
{
	GENERATED_BODY()
	
	virtual void HandleMatchHasStarted() override;
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_GameTime)
	float GameTime;

	UFUNCTION()
	void OnRep_GameTime();
	
	FOnGameTimeChanged OnGameTimeChanged;
};
