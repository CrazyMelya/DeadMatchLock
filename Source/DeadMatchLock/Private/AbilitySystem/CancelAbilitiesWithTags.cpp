// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CancelAbilitiesWithTags.h"

#include "AbilitySystem/DMLAbilitySystemComponent.h"
#include "Misc/DataValidation.h"

#define LOCTEXT_NAMESPACE "UCancelAbilitiesWithTags"

void UCancelAbilitiesWithTags::OnGameplayEffectApplied(FActiveGameplayEffectsContainer& ActiveGEContainer,
                                                       FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectApplied(ActiveGEContainer, GESpec, PredictionKey);
	
	ActiveGEContainer.Owner->CancelAbilities(&CancelAbilitiesTagsContainer.CombinedTags);
}

void UCancelAbilitiesWithTags::PostInitProperties()
{
	Super::PostInitProperties();
	const UCancelAbilitiesWithTags* Parent = FindParentComponent(*this);
	CancelAbilitiesTagsContainer.UpdateInheritedTagProperties(Parent ? &Parent->CancelAbilitiesTagsContainer : nullptr);
}

void UCancelAbilitiesWithTags::ApplyTargetTagChanges() const
{
	UGameplayEffect* Owner = GetOwner();
	CancelAbilitiesTagsContainer.ApplyTo(Owner->CachedGrantedTags);
}

void UCancelAbilitiesWithTags::OnGameplayEffectChanged()
{
	Super::OnGameplayEffectChanged();
	SetAndApplyTargetTagChanges(CancelAbilitiesTagsContainer);
}

void UCancelAbilitiesWithTags::SetAndApplyTargetTagChanges(const FInheritedTagContainer& TagContainerMods)
{
	CancelAbilitiesTagsContainer = TagContainerMods;

	// Try to find the parent and update the inherited tags
	const UCancelAbilitiesWithTags* Parent = FindParentComponent(*this);
	CancelAbilitiesTagsContainer.UpdateInheritedTagProperties(Parent ? &Parent->CancelAbilitiesTagsContainer : nullptr);

	// Apply to the owning Gameplay Effect Component
	ApplyTargetTagChanges();
}

#if WITH_EDITOR

void UCancelAbilitiesWithTags::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GetCancelAbilitiesTagsContainerName())
	{
		// Tell the GE it needs to reconfigure itself based on these updated properties (this will reaggregate the tags)
		UGameplayEffect* Owner = GetOwner();
		Owner->OnGameplayEffectChanged();
	}
}

EDataValidationResult UCancelAbilitiesWithTags::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	const bool bInstantEffect = (GetOwner()->DurationPolicy == EGameplayEffectDurationType::Instant);
	if (bInstantEffect && !CancelAbilitiesTagsContainer.CombinedTags.IsEmpty())
	{
		Context.AddError(FText::FormatOrdered(LOCTEXT("GEInstantAndTargetTags", "GE {0} is set to Instant so UCancelAbilitiesWithTags will not be able to apply its tags."), FText::FromString(GetNameSafe(GetOwner()))));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}

#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE