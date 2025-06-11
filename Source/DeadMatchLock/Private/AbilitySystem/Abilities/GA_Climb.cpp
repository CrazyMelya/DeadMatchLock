// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Climb.h"


void UGA_Climb::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (Character && IsLocallyControlled())
	{
		CommitAbility(Handle, ActorInfo, ActivationInfo);
	}
}

bool UGA_Climb::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) && SearchWall();
}

bool UGA_Climb::SearchWall() const
{
	FHitResult Hit;
	FVector Start = Character->GetActorLocation() + FVector(0, 0, MinClimbHeight);
	FVector End = Start + Character->GetActorForwardVector() * 50;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);
	Params.TraceTag = "Wall";
#if !UE_BUILD_SHIPPING
	GetWorld()->DebugDrawTraceTag = TEXT("Wall");
#endif
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params) && FVector::DotProduct(Hit.Normal, FVector::UpVector) < 0.1
		&& FVector::DotProduct(Hit.Normal, Character->GetActorForwardVector()) < -0.7)
	{
		FHitResult SecondHit;
		Start = Hit.Location + FVector(0, 0, MaxClimbHeight + 185) + Character->GetActorForwardVector() * 5;
		End = Hit.Location;
		Params.TraceTag = "WallHeight";
#if !UE_BUILD_SHIPPING
		GetWorld()->DebugDrawTraceTag = TEXT("WallHeight");
#endif
		if (GetWorld()->LineTraceSingleByChannel(SecondHit, Start, End, ECC_Visibility, Params))
		{
			float Height = FVector::Distance(Hit.Location, SecondHit.Location);
			return Height < MaxClimbHeight;
		}
		return false;
	}
	return false;
}
