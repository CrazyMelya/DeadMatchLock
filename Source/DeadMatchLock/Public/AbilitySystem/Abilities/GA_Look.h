// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DMLCharacter.h"
#include "AbilitySystem/DMLGameplayAbility.h"
#include "GA_Look.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UGA_Look : public UDMLGameplayAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
