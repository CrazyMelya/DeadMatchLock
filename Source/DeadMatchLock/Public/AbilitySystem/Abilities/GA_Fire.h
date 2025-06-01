// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBullet.h"
#include "DMLCharacter.h"
#include "AbilitySystem/Abilities/DMLGameplayAbility.h"
#include "GA_Fire.generated.h"

/**
 * 
 */
USTRUCT()
struct FBulletData
{
	GENERATED_BODY()

	UPROPERTY()
	uint32 BulletID;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;
};

UCLASS(Blueprintable)
class DEADMATCHLOCK_API UGA_Fire : public UDMLGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure)
	FVector CalculateFireTargetLocation() const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire")
	TSubclassOf<ABaseBullet> BulletClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Fire")
	float TraceDistance;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	
	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(Server, Reliable)
	void Fire_Server(const TArray<FBulletData>& BulletDataArray, const float ClientTime);

	bool RewindTrace(const FVector& Start, const FVector& End, float Radius) const;

	ABaseBullet* SpawnBullet(bool bIsPredicted, uint32 InBulletID, const FVector& Location, const FRotator& Rotation) const;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void ApplyIncreaseSpreadEffect();

	float GetSpreadAngle() const;

	int GetBulletsPerShot() const;
};
