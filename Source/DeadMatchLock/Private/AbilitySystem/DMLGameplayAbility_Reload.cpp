// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DMLGameplayAbility_Reload.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CharactersAttributeSet.h"

void UDMLGameplayAbility_Reload::OnTimerCompleted_Implementation()
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		auto ReloadEffect = ReloadEffectClass.GetDefaultObject();
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectToSelf(ReloadEffect, 0, GetAbilitySystemComponentFromActorInfo()->MakeEffectContext());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

bool UDMLGameplayAbility_Reload::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bFound;
	auto Ammo = ActorInfo->AbilitySystemComponent->GetGameplayAttributeValue(UCharactersAttributeSet::GetAmmoAttribute(), bFound);
	auto MaxAmmo = ActorInfo->AbilitySystemComponent->GetGameplayAttributeValue(UCharactersAttributeSet::GetMaxAmmoAttribute(), bFound);
	if (Ammo >= MaxAmmo)
		return false;
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}
