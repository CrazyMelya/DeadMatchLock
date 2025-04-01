// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA_Movement.h"

void UGA_Movement::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (Character)
	{
		if (auto Controller = Character->GetController())
		{
			auto MovementVector = Character->GetMovementVector();
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement 
			Character->AddMovementInput(ForwardDirection, MovementVector.Y);
			Character->AddMovementInput(RightDirection, MovementVector.X);
		}
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
