// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA_Fire.h"

#include "AbilitySystemComponent.h"
#include "ClientPredictedActor.h"
#include "DMLCharacter.h"
#include "EngineUtils.h"
#include "AbilitySystem/CharactersAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UGA_Fire::Fire()
{
	uint32 BulletID = AClientPredictedActor::GenerateClientID(Character);
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

	if (SpawnBullet(BulletID, true, Location, Rotation))
	{
		CommitAbilityCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
		Fire_Server(BulletID, GetWorld()->GetGameState()->GetServerWorldTimeSeconds(), Location, Rotation);
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

void UGA_Fire::Fire_Server_Implementation(uint32 BulletID, float ClientTime, FVector Location, FRotator Rotation)
{
	auto DefaultBullet = BulletClass->GetDefaultObject<ABaseBullet>();
	float InitialSpeed = DefaultBullet->MovementComponent->InitialSpeed;
	float Radius = DefaultBullet->CollisionComponent->GetScaledSphereRadius();
	FVector Direction = Rotation.Vector();

	FVector Start = Location;;
	FVector End = Start + Direction * InitialSpeed * (GetWorld()->GetTimeSeconds() - ClientTime);
	if (Rewind(ClientTime, Start, End, Rotation, Radius))
	{
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
		return;
	}
	if (SpawnBullet(BulletID, false, End, Rotation))
	{
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

bool UGA_Fire::Rewind(float ClientTime, FVector Start, FVector End, FRotator Rotation, float Radius)
{
	TMap<ADMLCharacter*, FSavedFrame> Characters;
	for (TActorIterator<ADMLCharacter> It(GetWorld()); It; ++It)
	{
		FSavedFrame CurrentFrame;
		CurrentFrame.Location = (*It)->GetActorLocation();
		CurrentFrame.Timestamp = GetWorld()->GetTimeSeconds();
		Characters.Add(*It, CurrentFrame);
		(*It)->RewindToTime(ClientTime);
	}
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);
	QueryParams.TraceTag = "RewindTrace";
#if !UE_BUILD_SHIPPING
	GetWorld()->DebugDrawTraceTag = TEXT("RewindTrace");
#endif

	auto bHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(Radius), QueryParams);
	if (bHit)
	{
		if (auto Victim = Cast<ADMLCharacter>(HitResult.GetActor()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *BulletClass.GetDefaultObject()->DamageEffectClass->GetName()));
			auto Context = Character->GetAbilitySystemComponent()->MakeEffectContext();
			Character->GetAbilitySystemComponent()->BP_ApplyGameplayEffectToTarget(BulletClass.GetDefaultObject()->DamageEffectClass,
				Victim->GetAbilitySystemComponent(), 1, Context);
		}
	}
	for (auto CharacterFrame : Characters)
	{
		CharacterFrame.Key->SetActorLocation(CharacterFrame.Value.Location);
	}
	return bHit;
}

ABaseBullet* UGA_Fire::SpawnBullet(uint32 BulletID, bool bIsPredicted, FVector Location, FRotator Rotation)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Character;
	SpawnParameters.Owner = Character;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	SpawnParameters.CustomPreSpawnInitalization = [bIsPredicted, BulletID, ActorsToIgnore](AActor* Actor)
	{
		if (auto PA = Cast<AClientPredictedActor>(Actor))
		{
			PA->SetID(BulletID);
			PA->SetIsPredictedCopy(bIsPredicted);
		}
		if (auto Bullet = Cast<ABaseBullet>(Actor))
			Bullet->ActorsToIgnore = ActorsToIgnore;
	};
	return GetWorld()->SpawnActor<ABaseBullet>(BulletClass, Location, Rotation, SpawnParameters);
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
