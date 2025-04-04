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
	CostData.Add(FGameplayTag::RequestGameplayTag(FName("SetByCaller.Cost.Stamina")), -1.0f);
	CostGameplayEffectClass = UGE_StaminaCost::StaticClass();
}

void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!Character)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid character in %s"), *GetName());
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}
		
		if (TryWallJump()) return;
		
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
	
	Character->StopJumping();
}

bool UGA_Jump::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Character->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Walking)
		return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
	return true;
}

bool UGA_Jump::TryWallJump()
{
	if (!Character->WallJumpIsAllowed()) return false;

	FHitResult Hit;
	FVector Start = Character->GetActorLocation() - FVector(0, 0, 90);
	FVector End = Start - Character->GetLastMovementInputVector() * 100.0f;
	FCollisionQueryParams Params;
	Params.TraceTag = "WallJump";
	Params.AddIgnoredActor(Character);
#if !UE_BUILD_SHIPPING
	GetWorld()->DebugDrawTraceTag = TEXT("WallJump");
#endif
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		Character->WallJump((Character->GetLastMovementInputVector() + FVector::UpVector * 1.5).GetSafeNormal());
		return true;
	}
	return false;
}
