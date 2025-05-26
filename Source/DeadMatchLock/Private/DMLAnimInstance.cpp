// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLAnimInstance.h"

#include "AbilitySystemGlobals.h"
#include "DMLCharacter.h"
#include "DMLCharacterMovementComponent.h"
#include "Misc/DataValidation.h"

UDMLAnimInstance::UDMLAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDMLAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	check(ASC);

	GameplayTagPropertyMap.Initialize(this, ASC);
}

#if WITH_EDITOR
EDataValidationResult UDMLAnimInstance::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	GameplayTagPropertyMap.IsDataValid(this, Context);

	return ((Context.GetNumErrors() > 0) ? EDataValidationResult::Invalid : EDataValidationResult::Valid);
}
#endif // WITH_EDITOR

void UDMLAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
}

void UDMLAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	const ADMLCharacter* Character = Cast<ADMLCharacter>(GetOwningActor());
	if (!Character)
	{
		return;
	}

	UDMLCharacterMovementComponent* CharMoveComp = CastChecked<UDMLCharacterMovementComponent>(Character->GetCharacterMovement());
	const FDMLCharacterGroundInfo& GroundInfo = CharMoveComp->GetGroundInfo();
	GroundDistance = GroundInfo.GroundDistance;
}

