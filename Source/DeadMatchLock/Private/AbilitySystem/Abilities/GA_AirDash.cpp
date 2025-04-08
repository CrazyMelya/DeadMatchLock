// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_AirDash.h"

#include "GameFramework/CharacterMovementComponent.h"


bool UGA_AirDash::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                     const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Character && Character->GetCharacterMovement()->MovementMode == MOVE_Falling && Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}
