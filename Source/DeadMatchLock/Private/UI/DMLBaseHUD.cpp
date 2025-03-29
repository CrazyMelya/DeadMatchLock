// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DMLBaseHUD.h"
#include "Abilities/GameplayAbility.h"

#include "DMLCharacter.h"
#include "AbilitySystem/CharactersAttributeSet.h"
#include "Net/UnrealNetwork.h"

void UDMLBaseHUD::SetTargetCharacter_Implementation(ADMLCharacter* NewTargetCharacter)
{
	if (NewTargetCharacter == nullptr || NewTargetCharacter == TargetCharacter) return;
	
	TargetCharacter = NewTargetCharacter;
	if (auto AbilitySystem = Cast<UDMLAbilitySystemComponent>(TargetCharacter->GetAbilitySystemComponent()))
	{
		bool bFound;
		BIND_ATTRIBUTE_VALUE_CHANGED(Health, AbilitySystem, bFound);
		BIND_ATTRIBUTE_VALUE_CHANGED(MaxHealth, AbilitySystem, bFound);
		BIND_ATTRIBUTE_VALUE_CHANGED(HealthRegen, AbilitySystem, bFound);
		BIND_ATTRIBUTE_VALUE_CHANGED(Stamina, AbilitySystem, bFound);
		BIND_ATTRIBUTE_VALUE_CHANGED(MaxStamina, AbilitySystem, bFound);
		BIND_ATTRIBUTE_VALUE_CHANGED(Ammo, AbilitySystem, bFound);
		BIND_ATTRIBUTE_VALUE_CHANGED(MaxAmmo, AbilitySystem, bFound);
		AbilitySystem->AbilityActivatedCallbacks.AddUObject(this, &UDMLBaseHUD::OnAbilityActivated);
		AbilitySystem->AbilityEndedCallbacks.AddUObject(this, &UDMLBaseHUD::OnAbilityEnded);
	}
}

void UDMLBaseHUD::SetGameState(ADMLGameState* InGameState)
{
	GameState = InGameState;
	OnRep_GameState();
}

void UDMLBaseHUD::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, GameState);
}

void UDMLBaseHUD::OnAttributeChanged(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	auto Action = OnAttributeChangedActions.Find(Attribute.GetUProperty()->GetFName());
	if (Action != nullptr)
		(*Action)(OldValue, NewValue);
}

void UDMLBaseHUD::OnAbilityActivated_Implementation(UGameplayAbility* ActivatedAbility)
{
}

void UDMLBaseHUD::OnAbilityEnded_Implementation(UGameplayAbility* EndedAbility)
{
}

void UDMLBaseHUD::OnRep_GameState()
{
}
