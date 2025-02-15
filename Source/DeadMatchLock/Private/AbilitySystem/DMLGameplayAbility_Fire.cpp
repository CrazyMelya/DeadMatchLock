// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DMLGameplayAbility_Fire.h"

#include "AbilitySystemComponent.h"
#include "DMLCharacter.h"
#include "AbilitySystem/CharactersAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UDMLGameplayAbility_Fire::Fire()
{
	FVector Location = Character->GetFirePointLocation();
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Character->GetFirePointLocation(), CalculateFireTargetLocation());
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Character;
	SpawnParameters.Owner = Character;
	auto Bullet = GetWorld()->SpawnActorDeferred<ABaseBullet>(
		BulletClass,
		FTransform(Rotation, Location),
		Character,
		Character);
	if (Bullet)
	{
		TArray<AActor*> ActorsToIgnore;
		Bullet->ActorsToIgnore = ActorsToIgnore;
		Bullet->FinishSpawning(FTransform(Rotation, Location));
		CommitAbilityCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		bool bFound;
		auto CurrentAmmo = Character->AbilitySystemComponent->GetGameplayAttributeValue(UCharactersAttributeSet::GetAmmoAttribute(), bFound);
		if (bFound)
		{
			if (CurrentAmmo <= 0)
			{
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
			}
		}
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

FVector UDMLGameplayAbility_Fire::CalculateFireTargetLocation()
{
	FVector Start = Character->GetFollowCamera()->GetComponentLocation();
	FVector End = Character->GetFollowCamera()->GetComponentLocation() + Character->GetFollowCamera()->GetForwardVector() * TraceDistance;
	FCollisionQueryParams Params;
	Params.TraceTag = "FireTrace";
	Params.AddIgnoredActor(Character);
	FHitResult Hit;
#if !UE_BUILD_SHIPPING
	GetWorld()->DebugDrawTraceTag = TEXT("FireTrace");
#endif
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		return Hit.Location;
	}
	return End;
}

float UDMLGameplayAbility_Fire::GetCooldownDuration_Implementation() const
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(FireTimer);
}

void UDMLGameplayAbility_Fire::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          bool bReplicateEndAbility, bool bWasCancelled)
{
	CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDMLGameplayAbility_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (HasAuthority(&ActivationInfo))
	{
		Character = Cast<ADMLCharacter>(GetAvatarActorFromActorInfo());
		if (Character)
		{
			bool bFound;
			auto FireTime = GetAbilitySystemComponentFromActorInfo()->GetGameplayAttributeValue(UCharactersAttributeSet::GetFireRateAttribute(), bFound);
			if (bFound)
				GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &UDMLGameplayAbility_Fire::Fire, FireTime, true);
		}
		else
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}
