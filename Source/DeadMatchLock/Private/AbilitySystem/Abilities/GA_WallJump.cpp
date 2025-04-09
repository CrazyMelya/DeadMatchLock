// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_WallJump.h"

#include "GameFramework/CharacterMovementComponent.h"

void UGA_WallJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (IsLocallyControlled())
	{
		if (Character->GetLastMovementInputVector() == FVector::ZeroVector)
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		FHitResult Hit;
		FVector Start = Character->GetActorLocation() - FVector(0, 0, 90);
		FVector End = Start - Character->GetLastMovementInputVector() * 100.0f;
		FCollisionQueryParams Params;
		Params.TraceTag = "WallJump";
		Params.AddIgnoredActor(Character);
#if !UE_BUILD_SHIPPING
		GetWorld()->DebugDrawTraceTag = TEXT("WallJump");
#endif
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params) && FVector::DotProduct(Hit.Normal, FVector::UpVector) < 0.1)
		{
			PerformJump((Character->GetLastMovementInputVector() + FVector::UpVector * 1.5).GetSafeNormal());
		}
		else
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

bool UGA_WallJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Character && Character->GetCharacterMovement()->IsFalling() && Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

