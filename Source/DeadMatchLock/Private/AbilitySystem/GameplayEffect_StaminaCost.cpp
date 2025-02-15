// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayEffect_StaminaCost.h"
#include "AbilitySystem/CharactersAttributeSet.h"

UGameplayEffect_StaminaCost::UGameplayEffect_StaminaCost(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FSetByCallerFloat Value;
	Value.DataTag = FGameplayTag::RequestGameplayTag(FName("SetByCaller.Cost.Stamina"));
	FGameplayEffectModifierMagnitude Data(Value);
	FGameplayModifierInfo Info;
	Info.ModifierMagnitude = Data;
	Info.ModifierOp = EGameplayModOp::Additive;
	Info.Attribute = UCharactersAttributeSet::GetStaminaAttribute();
	Modifiers.Add(Info);
}
