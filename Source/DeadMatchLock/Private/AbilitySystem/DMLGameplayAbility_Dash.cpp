// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DMLGameplayAbility_Dash.h"

#include "DMLCharacter.h"
#include "AbilitySystem/GameplayEffect_StaminaCost.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"

UDMLGameplayAbility_Dash::UDMLGameplayAbility_Dash(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CostData.Add(FGameplayTag::RequestGameplayTag(FName("SetByCaller.Cost.Stamina")), -1.0f);
	CostGameplayEffectClass = UGameplayEffect_StaminaCost::StaticClass();
	EndOnReleaseInnput = false;
}

void UDMLGameplayAbility_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (IsLocallyControlled())
	{
		ADMLCharacter* Character = CastChecked<ADMLCharacter>(ActorInfo->AvatarActor.Get());
		if (!Character)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid character in %s"), *GetName());
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}
		// if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		// {
		// 	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		// 	return;
		// }
		PerformDash(Character->GetLastMovementInputVector());
	}
}

void UDMLGameplayAbility_Dash::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!EndOnReleaseInnput) return;
	
	if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}