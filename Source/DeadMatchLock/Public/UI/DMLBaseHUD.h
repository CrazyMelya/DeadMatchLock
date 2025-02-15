// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/CharactersAttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "DMLBaseHUD.generated.h"

#define ADD_ATTRIBUTE_CHANGED_FUNC(AttributeName) \
OnAttributeChangedActions.Add(FName(#AttributeName), [this](float OldValue, float NewValue)\
{ \
	this->BP_On##AttributeName##ValueChanged(OldValue, NewValue); \
});

#define DECLARE_ATTRIBUTE_CHANGED_FUNC(AttributeName)\
void On##AttributeName##ValueChanged(const FOnAttributeChangeData& AttributeChangeData)\
{\
	BP_On##AttributeName##ValueChanged(AttributeChangeData.OldValue, AttributeChangeData.NewValue);\
};

#define BIND_ATTRIBUTE_VALUE_CHANGED(AttributeName, AbilitySystem, bFound)\
(AbilitySystem)->GetGameplayAttributeValueChangeDelegate(UCharactersAttributeSet::Get##AttributeName##Attribute())\
.AddUObject(this, &UDMLBaseHUD::On##AttributeName##ValueChanged);\
BP_On##AttributeName##ValueChanged(0, (AbilitySystem)->GetGameplayAttributeValue(UCharactersAttributeSet::Get##AttributeName##Attribute(), bFound))
/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UDMLBaseHUD : public UUserWidget
{
	GENERATED_BODY()

	TMap<FName, TFunction<void(float, float)>> OnAttributeChangedActions;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetTargetCharacter(ADMLCharacter* NewTargetCharacter);
	void SetTargetCharacter_Implementation(ADMLCharacter* NewTargetCharacter);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Attributes", DisplayName = "OnHealthValueChanged")
	void BP_OnHealthValueChanged(float OldValue, float NewValue);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Attributes", DisplayName = "OnMaxHealthValueChanged")
	void BP_OnMaxHealthValueChanged(float OldValue, float NewValue);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Attributes", DisplayName = "OnHealthRegenValueChanged")
	void BP_OnHealthRegenValueChanged(float OldValue, float NewValue);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Attributes", DisplayName = "OnStaminaValueChanged")
	void BP_OnStaminaValueChanged(float OldValue, float NewValue);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Attributes", DisplayName = "OnMaxStaminaValueChanged")
	void BP_OnMaxStaminaValueChanged(float OldValue, float NewValue);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Attributes", DisplayName = "OnAmmoValueChanged")
	void BP_OnAmmoValueChanged(float OldValue, float NewValue);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Attributes", DisplayName = "OnMaxAmmoValueChanged")
	void BP_OnMaxAmmoValueChanged(float OldValue, float NewValue);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ADMLCharacter* TargetCharacter;

	UFUNCTION()
	void OnAttributeChanged(const FGameplayAttribute& Attribute, float OldValue, float NewValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Abilities")
	void OnAbilityActivated(UGameplayAbility* ActivatedAbility);
	void OnAbilityActivated_Implementation(UGameplayAbility* ActivatedAbility);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Abilities")
	void OnAbilityEnded(UGameplayAbility* ActivatedAbility);
	void OnAbilityEnded_Implementation(UGameplayAbility* EndedAbility);

	DECLARE_ATTRIBUTE_CHANGED_FUNC(Health);
	DECLARE_ATTRIBUTE_CHANGED_FUNC(MaxHealth);
	DECLARE_ATTRIBUTE_CHANGED_FUNC(HealthRegen);
	DECLARE_ATTRIBUTE_CHANGED_FUNC(Stamina);
	DECLARE_ATTRIBUTE_CHANGED_FUNC(MaxStamina);
	DECLARE_ATTRIBUTE_CHANGED_FUNC(Ammo);
	DECLARE_ATTRIBUTE_CHANGED_FUNC(MaxAmmo);
};
