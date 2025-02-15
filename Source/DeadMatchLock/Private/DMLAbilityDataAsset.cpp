// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLAbilityDataAsset.h"

void UDMLAbilityDataAsset::GiveAbilities(UAbilitySystemComponent* ASC) const
{
	if (ASC)
	{
		for (auto Ability : Abilities)
		{
			if (Ability.AbilityClass)
			{
				ASC->GiveAbility(FGameplayAbilitySpec(Ability.AbilityClass, 1, static_cast<int32>(Ability.InputID)));
			}
		}
	}
}
