// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GA_Fire.h"

#include "AbilitySystemComponent.h"
#include "ClientPredictedActor.h"
#include "DMLCharacter.h"
#include "DMLCharacterMovementComponent.h"
#include "EngineUtils.h"
#include "AbilitySystem/CharactersAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/KismetMathLibrary.h"

void UGA_Fire::Fire()
{
	TArray<FBulletData> BulletDataArray;
	for (int i = 0; i < GetBulletsPerShot(); i++)
	{
		FBulletData BulletData;
		BulletData.BulletID = AClientPredictedActor::GenerateClientID(Character);
		BulletData.Location = Character->GetFirePointLocation();
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Character->GetFirePointLocation(), CalculateFireTargetLocation());
		FVector Direction = Rotation.Vector();
		Direction = FMath::VRandCone(Direction, FMath::DegreesToRadians(GetSpreadAngle()));
		BulletData.Rotation = Direction.Rotation();
		BulletDataArray.Add(BulletData);
		SpawnBullet(true, BulletData.BulletID, BulletData.Location, BulletData.Rotation);
	}
	if (!HasAuthority(&CurrentActivationInfo))
		Fire_Server(BulletDataArray, GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
	PlayAnimation();
	ApplyIncreaseSpreadEffect();
	CommitAbilityCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
}

void UGA_Fire::Fire_Server_Implementation(const TArray<FBulletData>& BulletDataArray, const float ClientTime)
{
	TMap<ADMLCharacter*, FSavedFrame> Characters;
	for (TActorIterator<ADMLCharacter> It(GetWorld()); It; ++It)
	{
		FSavedFrame CurrentFrame;
		CurrentFrame.Location = (*It)->GetActorLocation();
		CurrentFrame.Rotation = (*It)->GetActorRotation();
		CurrentFrame.CapsuleRadius = (*It)->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
		CurrentFrame.CapsuleHalfHeight = (*It)->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		CurrentFrame.Timestamp = GetWorld()->GetTimeSeconds();
		Characters.Add(*It, CurrentFrame);
		(*It)->RewindToTime(ClientTime);
	}
	const auto DefaultBullet = BulletClass->GetDefaultObject<ABaseBullet>();
	const float InitialSpeed = DefaultBullet->MovementComponent->InitialSpeed;
	const float Radius = DefaultBullet->CollisionComponent->GetScaledSphereRadius();
	for (auto BulletData : BulletDataArray)
	{
		const FVector Direction = BulletData.Rotation.Vector();
		const FVector Start = BulletData.Location;;
		const FVector End = Start + Direction * InitialSpeed * (GetWorld()->GetTimeSeconds() - ClientTime);
		if (!RewindTrace(Start, End, Radius))
		{
			SpawnBullet(false, BulletData.BulletID, End, BulletData.Rotation);
		}
	}
	for (auto CharacterFrame : Characters)
	{
		CharacterFrame.Key->SetActorLocation(CharacterFrame.Value.Location);
		CharacterFrame.Key->SetActorRotation(CharacterFrame.Value.Rotation);
		CharacterFrame.Key->GetCapsuleComponent()->SetCapsuleRadius(CharacterFrame.Value.CapsuleRadius);
		CharacterFrame.Key->GetCapsuleComponent()->SetCapsuleHalfHeight(CharacterFrame.Value.CapsuleHalfHeight);
	}
	ApplyIncreaseSpreadEffect();
	PlayAnimation();
	CommitAbilityCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
}

bool UGA_Fire::RewindTrace(const FVector& Start, const FVector& End, float Radius) const
{
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
	return bHit;
}

ABaseBullet* UGA_Fire::SpawnBullet(bool bIsPredicted, uint32 InBulletID, const FVector& Location, const FRotator& Rotation) const
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Character;
	SpawnParameters.Owner = Character;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	SpawnParameters.CustomPreSpawnInitalization = [bIsPredicted, InBulletID, ActorsToIgnore](AActor* Actor)
	{
		if (auto PA = Cast<AClientPredictedActor>(Actor))
		{
			PA->SetID(InBulletID);
			PA->SetIsPredictedCopy(bIsPredicted);
		}
		if (auto Bullet = Cast<ABaseBullet>(Actor))
			Bullet->ActorsToIgnore = ActorsToIgnore;
	};
	return GetWorld()->SpawnActor<ABaseBullet>(BulletClass, Location, Rotation, SpawnParameters);
}

float UGA_Fire::GetSpreadAngle() const
{
	return GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UCharactersAttributeSet::GetSpreadAttribute());
}

int UGA_Fire::GetBulletsPerShot() const
{
	return GetAbilitySystemComponentFromActorInfo()->GetNumericAttribute(UCharactersAttributeSet::GetBulletsPerShotAttribute());
}

FVector UGA_Fire::CalculateFireTargetLocation() const
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

void UGA_Fire::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          bool bReplicateEndAbility, bool bWasCancelled)
{
	CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (IsLocallyControlled())
	{
		if (Character)
		{
			Fire();
		}
		else
			CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}

void UGA_Fire::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	if (!Character->GetCharacterMovement<UDMLCharacterMovementComponent>()->GetIsSliding())
		Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
}
