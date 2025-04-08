// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/DMLGameplayAbility.h"
#include "GA_WallJump.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UGA_WallJump : public UDMLGameplayAbility
{
	GENERATED_BODY()
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(Server, Reliable)
	void WallJump_Server(FVector JumpDirection);
};
