// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DMLCharacter.h"
#include "AbilitySystem/Abilities/DMLGameplayAbility.h"
#include "GA_Move.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UGA_Move : public UDMLGameplayAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
