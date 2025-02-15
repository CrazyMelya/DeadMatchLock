// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DMLAbilitySystemComponent.h"
#include "DMLAbilityDataAsset.h"

void UDMLAbilitySystemComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if ((GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer) && AbilityDataAsset)
		AbilityDataAsset->GiveAbilities(this);
	AbilityActivatedCallbacks.AddUObject(this, &UDMLAbilitySystemComponent::OnAbilityActivatedCallback);
	OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &UDMLAbilitySystemComponent::OnEffectRemovedCallback);
}

void UDMLAbilitySystemComponent::RemoveAllActiveGameplayEffects()
{
	ActiveGameplayEffects.RemoveActiveEffects(FGameplayEffectQuery(), -1);
}

void UDMLAbilitySystemComponent::OnAbilityActivatedCallback(UGameplayAbility* Ability)
{
	OnAbilityActivated.Broadcast(Ability);
}

void UDMLAbilitySystemComponent::BP_AddLooseGameplayTag(const FGameplayTag TagToAdd)
{
	AddLooseGameplayTag(TagToAdd);
}

void UDMLAbilitySystemComponent::BP_AddLooseGameplayTags(const FGameplayTagContainer TagsToAdd)
{
	AddLooseGameplayTags(TagsToAdd);
}

void UDMLAbilitySystemComponent::BP_AddUniqueLooseGameplayTag(const FGameplayTag TagToAdd)
{
	AddLooseGameplayTag(TagToAdd);
	SetLooseGameplayTagCount(TagToAdd, 1);
}

void UDMLAbilitySystemComponent::BP_AddUniqueLooseGameplayTags(const FGameplayTagContainer TagsToAdd)
{
	AddLooseGameplayTags(TagsToAdd);
	for(auto TagsIterator = TagsToAdd.CreateConstIterator(); TagsIterator; ++TagsIterator)
	{
		SetLooseGameplayTagCount(*TagsIterator, 1);
	}
}

void UDMLAbilitySystemComponent::BP_RemoveLooseGameplayTag(const FGameplayTag TagToRemove)
{
	RemoveLooseGameplayTag(TagToRemove);
}

void UDMLAbilitySystemComponent::BP_RemoveLooseGameplayTags(const FGameplayTagContainer TagsToRemove)
{
	RemoveLooseGameplayTags(TagsToRemove);
}

void UDMLAbilitySystemComponent::BP_RemoveEntirelyLooseGameplayTag(const FGameplayTag TagToRemove)
{
	RemoveLooseGameplayTag(TagToRemove);
	SetLooseGameplayTagCount(TagToRemove, 0);
}

void UDMLAbilitySystemComponent::BP_RemoveEntirelyLooseGameplayTags(const FGameplayTagContainer TagsToRemove)
{
	RemoveLooseGameplayTags(TagsToRemove);
	for(auto TagsIterator = TagsToRemove.CreateConstIterator(); TagsIterator; ++TagsIterator)
	{
		SetLooseGameplayTagCount(*TagsIterator, 0);
	}
}

bool UDMLAbilitySystemComponent::BP_HasMatchingTag(const FGameplayTag TagToCheck) const
{
	return HasMatchingGameplayTag(TagToCheck);
}

void UDMLAbilitySystemComponent::OnEffectRemovedCallback(const FActiveGameplayEffect& RemovedGameplayEffect)
{
	OnEffectRemoved.Broadcast(RemovedGameplayEffect.Spec.Def);
}
