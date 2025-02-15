// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DMLGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CharactersAttributeSet.h"
#include "DMLDeveloperSettings.h"

void UDMLGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	const FGameplayEffectSpecHandle CDGEHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, CooldownGameplayEffectClass);
	if (!CDGEHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create CDGE for %s"), *GetName());
		return;
	}
	if (GetDefault<UDMLDeveloperSettings>()->CooldownTag == FGameplayTag::EmptyTag)
	{
		UE_LOG(LogTemp, Warning, TEXT("CooldownTag is not set in DMLDeveloperSettings."));
		return;
	}
	if (GetCooldownDuration() > 0 || CooldownTags.IsEmpty())
	{
		CDGEHandle.Data->DynamicGrantedTags.AppendTags(CooldownTags);
		CDGEHandle.Data->SetSetByCallerMagnitude(GetDefault<UDMLDeveloperSettings>()->CooldownTag, GetCooldownDuration());
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, CDGEHandle);
	}
}

float UDMLGameplayAbility::GetCooldownDuration_Implementation() const
{
	return CooldownDuration.Value;
}

FGameplayTagContainer* UDMLGameplayAbility::GetCooldownTags() const
{
	FGameplayTagContainer* LocalCooldownTags = new FGameplayTagContainer();
	if (const UGameplayEffect* CDGE = GetCooldownGameplayEffect())
		LocalCooldownTags->AppendTags(CDGE->GetGrantedTags());
	LocalCooldownTags->AppendTags(CooldownTags);
	return LocalCooldownTags;
}

void UDMLGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	
	const FGameplayEffectSpecHandle CostGEHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, CostGameplayEffectClass);
	if (!CostGEHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create CostGameplayEffect for %s"), *GetName());
		return;
	}
	if (CostData.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("CostData is empty for %s"), *GetName());
		return;
	}
	for (auto Cost : CostData)
	{
		CostGEHandle.Data->SetSetByCallerMagnitude(Cost.Key, Cost.Value.Value);
	}
	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, CostGEHandle);
}

bool UDMLGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	auto CostGE = GetCostGameplayEffect();
	auto AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	if (!AbilitySystemComponent)
		return false;
	if (CostGE)
	{
		FGameplayTagContainer GETags;
		for (auto Mod : CostGE->Modifiers)
		{
			GETags.AddTag(Mod.ModifierMagnitude.GetSetByCallerFloat().DataTag);
			if (Mod.ModifierOp == EGameplayModOp::Additive)
			{
				if (!Mod.Attribute.IsValid())
					continue;
				if (!CostData.Contains(Mod.ModifierMagnitude.GetSetByCallerFloat().DataTag))
					continue;
				auto AttributeSet = Cast<UCharactersAttributeSet>(AbilitySystemComponent->GetAttributeSet(Mod.Attribute.GetAttributeSetClass()));
				auto CostValue = CostData.Find(Mod.ModifierMagnitude.GetSetByCallerFloat().DataTag)->Value;
				auto CurrentValue = Mod.Attribute.GetNumericValueChecked(AttributeSet);
				if (CurrentValue + CostValue < AttributeSet->GetAttributeMinValue(Mod.Attribute))
					return false;
			}
		}
		for (auto Cost : CostData)
		{
			if (!GETags.HasTag(Cost.Key))
				return false;
		}
	}
	return true;
}

void UDMLGameplayAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!EndOnReleaseInnput) return;
	
	if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}
