// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effects/GE_StaminaRegen.h"
#include "AbilitySystem/CharactersAttributeSet.h"

UGE_StaminaRegen::UGE_StaminaRegen(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period.Value = 0.2f;
	FAttributeBasedFloat Value;
	Value.BackingAttribute.AttributeToCapture = UCharactersAttributeSet::GetStaminaRecoveryRateAttribute();
	Value.Coefficient = 0.2f;
	FGameplayEffectModifierMagnitude Data(Value);
	FGameplayModifierInfo Info;
	Info.ModifierMagnitude = Data;
	Info.ModifierOp = EGameplayModOp::Additive;
	Info.Attribute = UCharactersAttributeSet::GetStaminaAttribute();
	Modifiers.Add(Info);
}
