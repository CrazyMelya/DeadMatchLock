// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DMLGameplayAbility_Reload.h"

#include "AbilitySystemComponent.h"
#include "SharedTypes.h"
#include "AbilitySystem/CharactersAttributeSet.h"

void UDMLGameplayAbility_Reload::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	// GetAbilitySystemComponentFromActorInfo()->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Status.Stun"), EGameplayTagEventType::NewOrRemoved)
	// 	.AddUObject(this, &ThisClass::OnStunTagChanged);
}

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

void UDMLGameplayAbility_Reload::OnStunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	// if (NewCount > 0)
	// {
	// 	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	// }
	// else if (CanActivateAbility(CurrentSpecHandle, CurrentActorInfo))
	// if (NewCount <= 0)
	// {
	// 	ActivateAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, nullptr);
	// }
}

void UDMLGameplayAbility_Reload::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	GetAbilitySystemComponentFromActorInfo()->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(TAG_STUNNED), EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &ThisClass::OnStunTagChanged);
}
