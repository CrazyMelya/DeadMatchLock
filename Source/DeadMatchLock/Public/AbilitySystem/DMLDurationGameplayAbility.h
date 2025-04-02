// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/DMLGameplayAbility.h"
#include "DMLDurationGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UDMLDurationGameplayAbility : public UDMLGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnTimerCompleted();
	virtual void OnTimerCompleted_Implementation();

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category="Duration")
	FTimerHandle Timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Duration")
	FScalableFloat Duration;

public:
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Duration")
	float GetDuration();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Duration")
	float GetRemainingTime();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
