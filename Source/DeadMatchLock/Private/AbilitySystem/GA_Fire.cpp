// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA_Fire.h"

#include "AbilitySystemComponent.h"
#include "ClientPredictedActor.h"
#include "DMLCharacter.h"
#include "AbilitySystem/CharactersAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UGA_Fire::Fire()
{
	uint32 BulletID = AClientPredictedActor::GenerateClientID(Character);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Fire %i"), BulletID));
	FVector Location = Character->GetFirePointLocation();
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Character->GetFirePointLocation(), CalculateFireTargetLocation());
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Character;
	SpawnParameters.Owner = Character;
	bool bIsPredicted = true;
	SpawnParameters.CustomPreSpawnInitalization = [bIsPredicted, BulletID](AActor* Actor)
	{
		// Do the ID init here, before BeginPlay & replication
		if (auto PA = Cast<AClientPredictedActor>(Actor))
		{
			PA->SetID(BulletID);
			/// You should determine this value yourself based on whether this is the local creation, or the Server RPC
			/// It just sets the "bIsPredictedCopy" internal variable which lets us differentiate on the local client
			PA->SetIsPredictedCopy(bIsPredicted);
		}
	};
	// auto Bullet = GetWorld()->SpawnActorDeferred<ABaseBullet>(
	// 	BulletClass,
	// 	FTransform(Rotation, Location),
	// 	Character,
	// 	Character);
	if (auto Bullet = GetWorld()->SpawnActor<ABaseBullet>(BulletClass, Location, Rotation, SpawnParameters))
	{
		Bullet->ActorsToIgnore.Add(Character);
		// Bullet->FinishSpawning(FTransform(Rotation, Location));
		CommitAbilityCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		Fire_Server(BulletID);
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

void UGA_Fire::Fire_Server_Implementation(uint32 BulletID)
{
	FVector Location = Character->GetFirePointLocation();
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Character->GetFirePointLocation(), CalculateFireTargetLocation());
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Character;
	SpawnParameters.Owner = Character;
	bool bIsPredicted = false;
	SpawnParameters.CustomPreSpawnInitalization = [bIsPredicted, BulletID](AActor* Actor)
	{
		// Do the ID init here, before BeginPlay & replication
		if (auto PA = Cast<AClientPredictedActor>(Actor))
		{
			PA->SetID(BulletID);
			/// You should determine this value yourself based on whether this is the local creation, or the Server RPC
			/// It just sets the "bIsPredictedCopy" internal variable which lets us differentiate on the local client
			PA->SetIsPredictedCopy(bIsPredicted);
		}
	};
	// auto Bullet = GetWorld()->SpawnActorDeferred<ABaseBullet>(
	// 	BulletClass,
	// 	FTransform(Rotation, Location),
	// 	Character,
	// 	Character);
	if (auto Bullet = GetWorld()->SpawnActor<ABaseBullet>(BulletClass, Location, Rotation, SpawnParameters))
	{
		Bullet->ActorsToIgnore.Add(Character);
		// Bullet->FinishSpawning(FTransform(Rotation, Location));
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

FVector UGA_Fire::CalculateFireTargetLocation()
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

float UGA_Fire::GetCooldownDuration_Implementation() const
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(FireTimer);
}

void UGA_Fire::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          bool bReplicateEndAbility, bool bWasCancelled)
{
	CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (IsLocallyControlled())
	{
		Character = Cast<ADMLCharacter>(GetAvatarActorFromActorInfo());
		if (Character)
		{
			bool bFound;
			auto FireTime = GetAbilitySystemComponentFromActorInfo()->GetGameplayAttributeValue(UCharactersAttributeSet::GetFireRateAttribute(), bFound);
			if (bFound)
				GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &UGA_Fire::Fire, FireTime, true, 0);
		}
		else
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}
