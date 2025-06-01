// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CharactersAttributeSet.h"

#include "AbilitySystem/DMLAbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UCharactersAttributeSet::UCharactersAttributeSet()
{
	INIT_MAX_MAPS(Health);
	INIT_MAX_MAPS(Ammo);
	INIT_MAX_MAPS(Stamina);
	INIT_MAX_MAPS(AirJumps);
	INIT_MAX_MAPS(AirDashes);
	INIT_MAX_MAPS(Spread);
	INIT_MIN_MAPS(Spread);
}

void UCharactersAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, Health, OldHealth);
}

void UCharactersAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, MaxHealth, OldMaxHealth);
}

void UCharactersAttributeSet::OnRep_Ammo(const FGameplayAttributeData& OldAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, Ammo, OldAmmo);
}

void UCharactersAttributeSet::OnRep_MaxAmmo(const FGameplayAttributeData& OldMaxAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, MaxAmmo, OldMaxAmmo);
}

void UCharactersAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, Stamina, OldStamina);
}

void UCharactersAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, MaxStamina, OldMaxStamina);
}

void UCharactersAttributeSet::OnRep_JumpMaxCount(const FGameplayAttributeData& OldJumpMaxCount)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, JumpMaxCount, OldJumpMaxCount);
}

void UCharactersAttributeSet::OnRep_StaminaRecoveryRate(const FGameplayAttributeData& OldStaminaRecoveryRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, StaminaRecoveryRate, OldStaminaRecoveryRate);
}

void UCharactersAttributeSet::OnRep_BaseFireTime(const FGameplayAttributeData& OldBaseFireTime)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, StaminaRecoveryRate, OldBaseFireTime);
}

void UCharactersAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, HealthRegen, OldHealthRegen);
}

void UCharactersAttributeSet::OnRep_FireRate(const FGameplayAttributeData& OldFireRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, FireRate, OldFireRate);
}

void UCharactersAttributeSet::OnRep_AirJumps(const FGameplayAttributeData& OldAirJumps)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, AirJumps, OldAirJumps);
}

void UCharactersAttributeSet::OnRep_MaxAirJumps(const FGameplayAttributeData& OldMaxAirJumps)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, MaxAirJumps, OldMaxAirJumps);
}

void UCharactersAttributeSet::OnRep_AirDashes(const FGameplayAttributeData& OldAirDashes)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, AirDashes, OldAirDashes);
}

void UCharactersAttributeSet::OnRep_MaxAirDashes(const FGameplayAttributeData& OldMaxAirDashes)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, MaxAirDashes, OldMaxAirDashes);
}

void UCharactersAttributeSet::OnRep_WeaponDamage(const FGameplayAttributeData& OldWeaponDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, WeaponDamage, OldWeaponDamage);
}

void UCharactersAttributeSet::OnRep_MeleeDamage(const FGameplayAttributeData& OldMeleeDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, MeleeDamage, OldMeleeDamage);
}

void UCharactersAttributeSet::OnRep_Spread(const FGameplayAttributeData& OldSpread)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, Spread, OldSpread);
}

void UCharactersAttributeSet::OnRep_MinSpread(const FGameplayAttributeData& OldMinSpread)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, MinSpread, OldMinSpread);
}

void UCharactersAttributeSet::OnRep_MaxSpread(const FGameplayAttributeData& OldMaxSpread)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, MaxSpread, OldMaxSpread);
}

void UCharactersAttributeSet::OnRep_SpreadIncreaseRate(const FGameplayAttributeData& OldSpreadIncreaseRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, SpreadIncreaseRate, OldSpreadIncreaseRate);
}

void UCharactersAttributeSet::OnRep_BulletsPerShot(const FGameplayAttributeData& OldBulletsPerShot)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactersAttributeSet, BulletsPerShot, OldBulletsPerShot);
}

void UCharactersAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	NewValue = FMath::Clamp(NewValue, GetAttributeMinValue(Attribute), GetAttributeMaxValue(Attribute));
	Super::PreAttributeChange(Attribute, NewValue);
}

void UCharactersAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	NewValue = FMath::Clamp(NewValue, GetAttributeMinValue(Attribute), GetAttributeMaxValue(Attribute));
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UCharactersAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HealthRegen, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Ammo, COND_None, REPNOTIFY_OnChanged)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxAmmo, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Stamina, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxStamina, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, JumpMaxCount, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, BaseFireTime, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, FireRate, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, WeaponDamage, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, AirJumps, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxAirJumps, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, AirDashes, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxAirDashes, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MeleeDamage, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Spread, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MinSpread, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxSpread, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, SpreadIncreaseRate, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, BulletsPerShot, COND_None, REPNOTIFY_OnChanged);
}

float UCharactersAttributeSet::GetAttributeMaxValue(const FGameplayAttribute& Attribute) const
{
	auto MaxLimit = MaxLimitsMap.Find(Attribute);
	return MaxLimit ? MaxLimit->GetGameplayAttributeData(this)->GetCurrentValue() : FLT_MAX;
}

float UCharactersAttributeSet::GetAttributeMinValue(const FGameplayAttribute& Attribute) const
{
	auto MinLimit = MinLimitsMap.Find(Attribute);
	return MinLimit ? MinLimit->GetGameplayAttributeData(this)->GetCurrentValue() : 0;
}

void UCharactersAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (OldValue == NewValue) return;
	
	auto AbilitySystem = Cast<UDMLAbilitySystemComponent>(GetOwningAbilitySystemComponent());
	AbilitySystem->OnAttributeValueChanged.Broadcast(Attribute, OldValue, NewValue);
}

void UCharactersAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (GetHealth() <= GetAttributeMinValue(GetHealthAttribute()))
		{
			OnOutOfHealth.Broadcast(Data.EffectSpec.GetEffectContext().GetEffectCauser());
		}
	}
}
