// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DMLGameplayAbility.h"
#include "GA_Climb.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UGA_Climb : public UDMLGameplayAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Climb")
	float MinClimbHeight = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Climb")
	float MaxClimbHeight = 100.0f;

	UFUNCTION(BlueprintCallable)
	bool SearchWall() const;
};
