// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Look.h"

UGA_Look::UGA_Look(const FObjectInitializer& ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Look::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	Character = Cast<ADMLCharacter>(GetAvatarActorFromActorInfo());
}

void UGA_Look::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (Character)
	{
		auto LookVector = Character->GetLookVector();
		GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red, FString::Printf(TEXT("Look vector %f"), LookVector.Length()));
		// add yaw and pitch input to controller
		Character->AddControllerYawInput(LookVector.X);
		Character->AddControllerPitchInput(LookVector.Y);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
