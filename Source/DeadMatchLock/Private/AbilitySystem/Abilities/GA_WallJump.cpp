// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_WallJump.h"

void UGA_WallJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (IsLocallyControlled())
	{
		if (!Character || Character->GetLastMovementInputVector() == FVector::ZeroVector || !Character->WallJumpIsAllowed())
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
			Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
			FVector JumpDirection = (Character->GetLastMovementInputVector() + FVector::UpVector * 1.5).GetSafeNormal();
			Character->WallJump(JumpDirection);
			if (!HasAuthority(&ActivationInfo))
				WallJump_Server(JumpDirection);
		}
		else
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UGA_WallJump::WallJump_Server_Implementation(FVector JumpDirection)
{
	Character->WallJump(JumpDirection);
}

