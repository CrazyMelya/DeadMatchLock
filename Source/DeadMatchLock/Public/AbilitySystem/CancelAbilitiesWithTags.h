// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "GameplayEffect.h"
#include "CancelAbilitiesWithTags.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UCancelAbilitiesWithTags : public UGameplayEffectComponent
{
	GENERATED_BODY()

public:
	virtual void OnGameplayEffectChanged() override;
	
	void SetAndApplyTargetTagChanges(const FInheritedTagContainer& TagContainerMods);
	
#if WITH_EDITOR
	/** Needed to properly update FInheritedTagContainer properties */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/** Validate incompatible configurations */
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;

private:
	/** Get a cached version of the FProperty Name for PostEditChangeProperty */
	static const FName& GetCancelAbilitiesTagsContainerName()
	{
		static FName NAME_CancelAbilitiesTagsContainer = GET_MEMBER_NAME_CHECKED(UCancelAbilitiesWithTags, CancelAbilitiesTagsContainer);
		return NAME_CancelAbilitiesTagsContainer;
	}
#endif // WITH_EDITOR
	
protected:
	virtual void OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const;

	virtual void PostInitProperties() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = None, meta = (DisplayName = "Tags to Cancel"))
	FInheritedTagContainer CancelAbilitiesTagsContainer;
	
	void ApplyTargetTagChanges() const;
};
