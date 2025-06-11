// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharactersAttributeSet.generated.h"


/**
 * 
 */
#define ATTRIBUTE_ACCESSORS(PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UCharactersAttributeSet, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//#define DECLARE_ATTRIBUTE_WITH_LIMITS(PropertyName) \
//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes") \
//FGameplayAttributeData PropertyName; \
//ATTRIBUTE_ACCESSORS(PropertyName)\
//\
//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes") \
//FGameplayAttributeData Max##PropertyName; \
//ATTRIBUTE_ACCESSORS(Max##PropertyName) \
//\
//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes") \
//FGameplayAttributeData Min##PropertyName; \
//ATTRIBUTE_ACCESSORS(Min##PropertyName) \

#define INIT_MAX_MAPS(PropertyName) \
MaxLimitsMap.Add(Get##PropertyName##Attribute(), GetMax##PropertyName##Attribute());

#define INIT_MIN_MAPS(PropertyName) \
MinLimitsMap.Add(Get##PropertyName##Attribute(), GetMin##PropertyName##Attribute());

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOutOfHealth, AActor*)

UCLASS()
class DEADMATCHLOCK_API UCharactersAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCharactersAttributeSet();

private:
	TMap<FGameplayAttribute, FGameplayAttribute> MinLimitsMap;
	TMap<FGameplayAttribute, FGameplayAttribute> MaxLimitsMap;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(Health);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(MaxHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_HealthRegen)
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(HealthRegen);
	
	UFUNCTION()
	virtual void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Ammo)
	FGameplayAttributeData Ammo;
	ATTRIBUTE_ACCESSORS(Ammo);
	
	UFUNCTION()
	virtual void OnRep_Ammo(const FGameplayAttributeData& OldAmmo);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxAmmo)
	FGameplayAttributeData MaxAmmo;
	ATTRIBUTE_ACCESSORS(MaxAmmo);

	UFUNCTION()
	virtual void OnRep_MaxAmmo(const FGameplayAttributeData& OldMaxAmmo);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(Stamina);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(MaxStamina);
	
	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_JumpMaxCount)
	FGameplayAttributeData JumpMaxCount;
	ATTRIBUTE_ACCESSORS(JumpMaxCount);
	
	UFUNCTION()
	virtual void OnRep_JumpMaxCount(const FGameplayAttributeData& OldJumpMaxCount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_StaminaRecoveryRate)
	FGameplayAttributeData StaminaRecoveryRate;
	ATTRIBUTE_ACCESSORS(StaminaRecoveryRate);
	
	UFUNCTION()
	virtual void OnRep_StaminaRecoveryRate(const FGameplayAttributeData& OldStaminaRecoveryRate);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_BaseFireTime)
	FGameplayAttributeData BaseFireTime;
	ATTRIBUTE_ACCESSORS(BaseFireTime);
	
	UFUNCTION()
	virtual void OnRep_BaseFireTime(const FGameplayAttributeData& OldBaseFireTime);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_FireRate)
	FGameplayAttributeData FireRate;
	ATTRIBUTE_ACCESSORS(FireRate);
	
	UFUNCTION()
	virtual void OnRep_FireRate(const FGameplayAttributeData& OldFireRate);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_WeaponDamage)
	FGameplayAttributeData WeaponDamage;
	ATTRIBUTE_ACCESSORS(WeaponDamage);

	UFUNCTION()
	virtual void OnRep_WeaponDamage(const FGameplayAttributeData& OldWeaponDamage);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MeleeDamage)
	FGameplayAttributeData MeleeDamage;
	ATTRIBUTE_ACCESSORS(MeleeDamage);

	UFUNCTION()
	virtual void OnRep_MeleeDamage(const FGameplayAttributeData& OldMeleeDamage);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_AirJumps)
	FGameplayAttributeData AirJumps;
	ATTRIBUTE_ACCESSORS(AirJumps);

	UFUNCTION()
	virtual void OnRep_AirJumps(const FGameplayAttributeData& OldAirJumps);

	UPROPERTY(EditAnywhere,	BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MaxAirJumps)
	FGameplayAttributeData MaxAirJumps;
	ATTRIBUTE_ACCESSORS(MaxAirJumps);
	
	UFUNCTION()
	virtual void OnRep_MaxAirJumps(const FGameplayAttributeData& OldMaxAirJumps);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_AirDashes)
	FGameplayAttributeData AirDashes;
	ATTRIBUTE_ACCESSORS(AirDashes);

	UFUNCTION()
	virtual void OnRep_AirDashes(const FGameplayAttributeData& OldAirDashes);

	UPROPERTY(EditAnywhere,	BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MaxAirDashes)
	FGameplayAttributeData MaxAirDashes;
	ATTRIBUTE_ACCESSORS(MaxAirDashes);
	
	UFUNCTION()
	virtual void OnRep_MaxAirDashes(const FGameplayAttributeData& OldMaxAirDashes);

	UPROPERTY(EditAnywhere,	BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_Spread)
	FGameplayAttributeData Spread;
	ATTRIBUTE_ACCESSORS(Spread);

	UFUNCTION()
	virtual void OnRep_Spread(const FGameplayAttributeData& OldSpread);

	UPROPERTY(EditAnywhere,	BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MinSpread)
	FGameplayAttributeData MinSpread;
	ATTRIBUTE_ACCESSORS(MinSpread);

	UFUNCTION()
	virtual void OnRep_MinSpread(const FGameplayAttributeData& OldMinSpread);

	UPROPERTY(EditAnywhere,	BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_MaxSpread)
	FGameplayAttributeData MaxSpread;
	ATTRIBUTE_ACCESSORS(MaxSpread);

	UFUNCTION()
	virtual void OnRep_MaxSpread(const FGameplayAttributeData& OldMaxSpread);

	UPROPERTY(EditAnywhere,	BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_SpreadIncreaseRate)
	FGameplayAttributeData SpreadIncreaseRate;
	ATTRIBUTE_ACCESSORS(SpreadIncreaseRate);

	UFUNCTION()
	virtual void OnRep_SpreadIncreaseRate(const FGameplayAttributeData& OldSpreadIncreaseRate);

	UPROPERTY(EditAnywhere,	BlueprintReadOnly, Category = "Attribute", ReplicatedUsing = OnRep_BulletsPerShot)
	FGameplayAttributeData BulletsPerShot;
	ATTRIBUTE_ACCESSORS(BulletsPerShot);

	UFUNCTION()
	virtual void OnRep_BulletsPerShot(const FGameplayAttributeData& OldBulletsPerShot);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float &NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	float GetAttributeMaxValue(const FGameplayAttribute& Attribute) const;
	float GetAttributeMinValue(const FGameplayAttribute& Attribute) const;

	FOnOutOfHealth OnOutOfHealth;
};
