// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBullet.h"
#include "DMLCharacter.h"
#include "AbilitySystem/DMLGameplayAbility.h"
#include "GA_Fire.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DEADMATCHLOCK_API UGA_Fire : public UDMLGameplayAbility
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void Fire();

	FTimerHandle FireTimer;

protected:
	UFUNCTION(BlueprintPure)
	FVector CalculateFireTargetLocation();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire")
	TSubclassOf<ABaseBullet> BulletClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire")
	float TraceDistance;

	ADMLCharacter* Character;

	virtual float GetCooldownDuration_Implementation() const override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
