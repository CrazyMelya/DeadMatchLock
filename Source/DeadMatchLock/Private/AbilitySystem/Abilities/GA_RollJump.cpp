// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_RollJump.h"

#include "GameFramework/CharacterMovementComponent.h"


void UGA_RollJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (Character && IsLocallyControlled())
	{
		FVector JumpDirection = Character->GetCharacterMovement()->Velocity.GetSafeNormal2D();
		JumpDirection.Z = 0.35;
		JumpDirection.Normalize();
		Character->GetCharacterMovement()->Launch(JumpDirection * 1750);
		if (!HasAuthority(&ActivationInfo))
			Jump_Server(JumpDirection);
		CommitAbility(Handle, ActorInfo, ActivationInfo);
	}
}

void UGA_RollJump::Jump_Server_Implementation(FVector JumpDirection)
{
	Character->GetCharacterMovement()->Launch(JumpDirection * 1750);
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
}
