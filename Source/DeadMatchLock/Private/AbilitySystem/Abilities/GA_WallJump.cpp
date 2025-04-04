// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_WallJump.h"

bool UGA_WallJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Character || Character->GetLastMovementInputVector() == FVector::ZeroVector || !Character->WallJumpIsAllowed()) return false;

	FHitResult Hit;
	FVector Start = Character->GetActorLocation() - FVector(0, 0, 90);
	FVector End = Start - Character->GetLastMovementInputVector() * 100.0f;
	FCollisionQueryParams Params;
	Params.TraceTag = "WallJump";
	Params.AddIgnoredActor(Character);
#if !UE_BUILD_SHIPPING
	GetWorld()->DebugDrawTraceTag = TEXT("WallJump");
#endif

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "WallJump");
	return GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params) && Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGA_WallJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Character->WallJump((Character->GetLastMovementInputVector() + FVector::UpVector * 2).GetSafeNormal());

	// EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
