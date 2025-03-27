// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DMLCharacter.h"
#include "AbilitySystem/DMLGameplayAbility.h"
#include "GA_Movement.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UGA_Movement : public UDMLGameplayAbility
{
	GENERATED_BODY()
	
	UGA_Movement(const FObjectInitializer& ObjectInitializer);
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UPROPERTY()
	ADMLCharacter* Character = nullptr;
};
