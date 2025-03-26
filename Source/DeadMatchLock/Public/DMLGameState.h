// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DMLGameState.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameTimeChanged, float)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayersSorted);

UCLASS()
class DEADMATCHLOCK_API ADMLGameState : public AGameState
{
	GENERATED_BODY()

public:
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_GameTime)
	int32 GameTime;

	UFUNCTION()
	void OnRep_GameTime();

	UFUNCTION()
	void SortPlayerArray();

	UPROPERTY(BlueprintAssignable)
	FOnPlayersSorted OnPlayersSorted;
	
	FOnGameTimeChanged OnGameTimeChanged;

	UPROPERTY(Transient, BlueprintReadOnly, Category=GameState)
	TArray<TObjectPtr<class ADMLPlayerState>> DMLPlayerArray;

private:
	UFUNCTION()
	void OnPlayerKillsChanged(int32 NewKills);
};
