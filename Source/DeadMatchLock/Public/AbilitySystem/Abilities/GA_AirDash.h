// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DMLGameplayAbility.h"
#include "GA_AirDash.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UGA_AirDash : public UDMLGameplayAbility
{
	GENERATED_BODY()
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
};
