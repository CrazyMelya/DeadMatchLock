// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SharedTypes.h"
#include "GameFramework/PlayerState.h"
#include "DMLPlayerState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, float, Score);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatChanged, int, Stat);

UCLASS()
class DEADMATCHLOCK_API ADMLPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Character")
	FName CharacterName;

	UPROPERTY(BlueprintAssignable)
	FOnScoreChanged OnScoreChanged;

	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnKillsChanged;

	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnDeathsChanged;

	virtual void OnRep_Score() override;

	UFUNCTION(NetMulticast, Reliable)
	void SetPlayerScore(float InScore);

	UFUNCTION(NetMulticast, Reliable)
	void SetKills(int32 NewKills);

	UFUNCTION(NetMulticast, Reliable)
	void SetDeaths(int32 NewDeaths);

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_Kills)
	int32 Kills;

	UFUNCTION()
	void OnRep_Kills();

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_Deaths)
	int32 Deaths;

	UFUNCTION()
	void OnRep_Deaths();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
