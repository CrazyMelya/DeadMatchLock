// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DMLGameplayAbility.h"
#include "GA_Roll.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UGA_Roll : public UDMLGameplayAbility
{
	GENERATED_BODY()

	UGA_Roll(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void PerformDash(FVector DashDirection);
	
private:
	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
};
