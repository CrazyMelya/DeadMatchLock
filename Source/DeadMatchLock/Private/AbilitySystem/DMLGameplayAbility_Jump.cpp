// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DMLGameplayAbility_Jump.h"

#include "DMLCharacter.h"
#include "AbilitySystem/GameplayEffect_StaminaCost.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionJumpForce.h"
#include "AbilitySystem/GameplayEffect_AmmoCost.h"
#include "GameFramework/CharacterMovementComponent.h"

UDMLGameplayAbility_Jump::UDMLGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CostData.Add(FGameplayTag::RequestGameplayTag(FName("SetByCaller.Cost.Stamina")), -1.0f);
	CostGameplayEffectClass = UGameplayEffect_StaminaCost::StaticClass();
}

void UDMLGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		ADMLCharacter* Character = CastChecked<ADMLCharacter>(ActorInfo->AvatarActor.Get());
		if (!Character)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid character in %s"), *GetName());
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}
		if (Character->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Walking)
		{
			if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
			{
				return;
			}
		}
		Character->Jump();
	}
}

void UDMLGameplayAbility_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

bool UDMLGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	const ADMLCharacter* Character = CastChecked<ADMLCharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
		
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return (Character && Character->CanJump());
}

void UDMLGameplayAbility_Jump::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UDMLGameplayAbility_Jump::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	
	ADMLCharacter * Character = CastChecked<ADMLCharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}

bool UDMLGameplayAbility_Jump::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	const ADMLCharacter* Character = CastChecked<ADMLCharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	if (Character->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Walking)
		return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
	return true;
}
