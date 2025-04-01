// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Look.h"

void UGA_Look::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (Character)
	{
		auto LookVector = Character->GetLookVector();
		// add yaw and pitch input to controller
		Character->AddControllerYawInput(LookVector.X);
		Character->AddControllerPitchInput(LookVector.Y);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
