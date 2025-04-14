// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA_Fire.h"

#include "AbilitySystemComponent.h"
#include "ClientPredictedActor.h"
#include "DMLCharacter.h"
#include "EngineUtils.h"
#include "AbilitySystem/CharactersAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/KismetMathLibrary.h"

void UGA_Fire::Fire()
{
	uint32 BulletID = AClientPredictedActor::GenerateClientID(Character);
	FVector Location = Character->GetFirePointLocation();
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Character->GetFirePointLocation(), CalculateFireTargetLocation());
	if (SpawnBullet(true, BulletID, Location, Rotation))
	{
		PlayAnimation();
		if (!HasAuthority(&CurrentActivationInfo))
			Fire_Server(BulletID, Location, Rotation, GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
		CommitAbilityCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	}
}

void UGA_Fire::Fire_Server_Implementation(uint32 BulletID, FVector Location, FRotator Rotation, float ClientTime)
{
	auto DefaultBullet = BulletClass->GetDefaultObject<ABaseBullet>();
	float InitialSpeed = DefaultBullet->MovementComponent->InitialSpeed;
	float Radius = DefaultBullet->CollisionComponent->GetScaledSphereRadius();
	FVector Direction = Rotation.Vector();

	FVector Start = Location;;
	FVector End = Start + Direction * InitialSpeed * (GetWorld()->GetTimeSeconds() - ClientTime);
	Location = End;
	if (RewindAndTrace(ClientTime, Start, End, Radius) || SpawnBullet(false, BulletID, Location, Rotation))
	{
		PlayAnimation();
		CommitAbilityCost(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	}
}

bool UGA_Fire::RewindAndTrace(float ClientTime, FVector Start, FVector End, float Radius) const
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
