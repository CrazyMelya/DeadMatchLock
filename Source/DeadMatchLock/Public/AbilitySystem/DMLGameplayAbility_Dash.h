// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DMLGameplayAbility.h"
#include "DMLGameplayAbility_Dash.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UDMLGameplayAbility_Dash : public UDMLGameplayAbility
{
	GENERATED_BODY()

	UDMLGameplayAbility_Dash(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void PerformDash(FVector DashDirection);
	
private:
	virtual void ActivateAbility(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayAbilityActivationInfo ActivationInfo) override;
};
