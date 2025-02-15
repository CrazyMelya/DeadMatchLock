// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DMLAbilityDataAsset.h"
#include "DMLAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, const FGameplayAttribute&, Attribute, float, OldValue, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityActivated, UGameplayAbility*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectRemoved, const UGameplayEffect*, RemovedEffect);

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UDMLAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	UDMLAbilityDataAsset* AbilityDataAsset;
	
	virtual void InitializeComponent() override;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnAttributeValueChanged;

	UPROPERTY(BlueprintAssignable, Category = "Abilities")
	FOnAbilityActivated OnAbilityActivated;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void RemoveAllActiveGameplayEffects();
	
	UFUNCTION()
	void OnAbilityActivatedCallback(UGameplayAbility* Ability);

	UFUNCTION(BlueprintCallable, Category = "GameplayTags", DisplayName = "AddLooseGameplayTag")
	void BP_AddLooseGameplayTag(const FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "GameplayTags", DisplayName = "AddLooseGameplayTags")
	void BP_AddLooseGameplayTags(const FGameplayTagContainer TagsToAdd);

	UFUNCTION(BlueprintCallable, Category = "GameplayTags", DisplayName = "AddUniqueLooseGameplayTag")
	void BP_AddUniqueLooseGameplayTag(const FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "GameplayTags", DisplayName = "AddUniqueLooseGameplayTags")
	void BP_AddUniqueLooseGameplayTags(const FGameplayTagContainer TagsToAdd);

	UFUNCTION(BlueprintCallable, Category = "GameplayTags", DisplayName = "RemoveLooseGameplayTag")
	void BP_RemoveLooseGameplayTag(const FGameplayTag TagToRemove);

	UFUNCTION(BlueprintCallable, Category = "GameplayTags", DisplayName = "RemoveLooseGameplayTags")
	void BP_RemoveLooseGameplayTags(const FGameplayTagContainer TagsToRemove);
	
	UFUNCTION(BlueprintCallable, Category = "GameplayTags", DisplayName = "RemoveEntirelyLooseGameplayTag")
	void BP_RemoveEntirelyLooseGameplayTag(const FGameplayTag TagToRemove);

	UFUNCTION(BlueprintCallable, Category = "GameplayTags", DisplayName = "RemoveEntirelyLooseGameplayTags")
	void BP_RemoveEntirelyLooseGameplayTags(const FGameplayTagContainer TagsToRemove);

	UFUNCTION(BlueprintPure, Category = "GameplayTags", DisplayName = "Has Matching Tag")
	bool BP_HasMatchingTag(const FGameplayTag TagToCheck) const;

	UPROPERTY(BlueprintAssignable)
	FOnEffectRemoved OnEffectRemoved;

	void OnEffectRemovedCallback(const FActiveGameplayEffect& RemovedGameplayEffect);
	
};
