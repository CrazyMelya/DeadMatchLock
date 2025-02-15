// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayEffect_AmmoCost.h"
#include "AbilitySystem/CharactersAttributeSet.h"

UGameplayEffect_AmmoCost::UGameplayEffect_AmmoCost(const FObjectInitializer& ObjectInitializer)
{
	FSetByCallerFloat Value;
	Value.DataTag = FGameplayTag::RequestGameplayTag(FName("SetByCaller.Cost.Ammo"));
	FGameplayEffectModifierMagnitude Data(Value);
	FGameplayModifierInfo Info;
	Info.ModifierMagnitude = Data;
	Info.ModifierOp = EGameplayModOp::Additive;
	Info.Attribute = UCharactersAttributeSet::GetAmmoAttribute();
	Modifiers.Add(Info);
}
