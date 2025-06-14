// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DMLDurationGameplayAbility.h"
#include "GA_Reload.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UGA_Reload : public UDMLDurationGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Effect")
	TSubclassOf<UGameplayEffect> ReloadEffectClass;


	virtual void OnTimerCompleted_Implementation() override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	FOnGameplayEffectTagCountChanged OnStunTagCountChanged;
};
