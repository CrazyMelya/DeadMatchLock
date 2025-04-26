// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/DMLDurationGameplayAbility.h"

void UDMLDurationGameplayAbility::OnTimerCompleted_Implementation()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

float UDMLDurationGameplayAbility::GetDuration()
{
	return Duration.Value;
}

float UDMLDurationGameplayAbility::GetRemainingTime()
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(Timer);
}

void UDMLDurationGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &UDMLDurationGameplayAbility::OnTimerCompleted, Duration.Value, false);
	}
}
