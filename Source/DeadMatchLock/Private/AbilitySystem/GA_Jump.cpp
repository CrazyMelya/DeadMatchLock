// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA_Jump.h"

#include "DMLCharacter.h"
#include "AbilitySystem/GE_StaminaCost.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionJumpForce.h"
#include "AbilitySystem/GE_AmmoCost.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_Jump::UGA_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CostData.Add(FGameplayTag::RequestGameplayTag(FName("SetByCaller.Cost.Stamina")), -1.0f);
	CostGameplayEffectClass = UGE_StaminaCost::StaticClass();
}

void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

void UGA_Jump::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

bool UGA_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	const ADMLCharacter* Character = CastChecked<ADMLCharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
		
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return (Character && Character->CanJump());
}

void UGA_Jump::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UGA_Jump::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	
	ADMLCharacter * Character = CastChecked<ADMLCharacter>(ActorInfo->AvatarActor.Get());
	Character->StopJumping();
}

bool UGA_Jump::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	const ADMLCharacter* Character = CastChecked<ADMLCharacter>(ActorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	if (Character->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Walking)
		return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
	return true;
}
