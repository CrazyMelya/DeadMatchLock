// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DMLGameplayAbility.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCostData
{
	GENERATED_BODY()

	FCostData()
		: CostTag(FGameplayTag::EmptyTag)
		,Magnitude(0.0f)
	{}
	
	FCostData(FGameplayTag InCostTag, FScalableFloat InMagnitude)
		: CostTag(InCostTag)
		, Magnitude(InMagnitude)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Costs", meta=(Categories="SetByCaller"))
	FGameplayTag CostTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Costs")
	FScalableFloat Magnitude;
};
 
UCLASS()
class DEADMATCHLOCK_API UDMLGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cooldowns")
	FScalableFloat CooldownDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cooldowns", meta=(Categories="Cooldown"))
	FGameplayTagContainer CooldownTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Costs", meta=(Categories="SetByCaller.Cost"))
	TMap<FGameplayTag, FScalableFloat> CostData;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Cooldown")
	float GetCooldownDuration() const;
	virtual float GetCooldownDuration_Implementation() const;

	virtual FGameplayTagContainer* GetCooldownTags() const override;

	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	bool EndOnReleaseInnput = true;
};
