// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effects/GE_ResetJumpState.h"

#include "AbilitySystem/CharactersAttributeSet.h"


UGE_ResetJumpState::UGE_ResetJumpState()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	// Restore Air Jumps
	FAttributeBasedFloat AirJumpsValue;
	AirJumpsValue.BackingAttribute.AttributeToCapture = UCharactersAttributeSet::GetMaxAirJumpsAttribute();
	FGameplayEffectModifierMagnitude AirJumpsData(AirJumpsValue);
	FGameplayModifierInfo AirJumpsInfo;
	AirJumpsInfo.ModifierMagnitude = AirJumpsData;
	AirJumpsInfo.ModifierOp = EGameplayModOp::Override;
	AirJumpsInfo.Attribute = UCharactersAttributeSet::GetAirJumpsAttribute();
	Modifiers.Add(AirJumpsInfo);

	// Restore Air Dashes
	FAttributeBasedFloat AirDashesValue;
	AirDashesValue.BackingAttribute.AttributeToCapture = UCharactersAttributeSet::GetMaxAirDashesAttribute();
	FGameplayEffectModifierMagnitude AirDashesData(AirDashesValue);
	FGameplayModifierInfo AirDashesInfo;
	AirDashesInfo.ModifierMagnitude = AirDashesData;
	AirDashesInfo.ModifierOp = EGameplayModOp::Override;
	AirDashesInfo.Attribute = UCharactersAttributeSet::GetAirDashesAttribute();
	Modifiers.Add(AirDashesInfo);
}
