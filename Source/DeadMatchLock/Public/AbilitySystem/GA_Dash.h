// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DMLGameplayAbility.h"
#include "GA_Dash.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UGA_Dash : public UDMLGameplayAbility
{
	GENERATED_BODY()

	UGA_Dash(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void PerformDash(FVector DashDirection);
	
private:
	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayAbilityActivationInfo ActivationInfo) override;
};
