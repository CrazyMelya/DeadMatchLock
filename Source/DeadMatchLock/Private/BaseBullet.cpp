// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBullet.h"

#include "DMLCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ABaseBullet::ABaseBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseBullet::OnOverlap);
	CollisionComponent->SetUseCCD(true);
	RootComponent = CollisionComponent;
	
    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
    MovementComponent->MaxSpeed = 1000.0f;
    MovementComponent->InitialSpeed = 1000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;
    MovementComponent->bSweepCollision = true;
	MovementComponent->UpdatedComponent= CollisionComponent;
}

// Called when the game starts or when spawned
void ABaseBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseBullet::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
}

// Called every frame
void ABaseBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseBullet::OnOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorsToIgnore.Contains(OtherActor)) return;

	if (HasAuthority())
	{
		if (auto Victim = Cast<ADMLCharacter>(OtherActor))
		{
			if (auto DamageInstigator = Cast<IAbilitySystemInterface>(GetInstigator()))
			{
				if (auto AbilitySystem = DamageInstigator->GetAbilitySystemComponent())
				{
					auto DamageEffect = DamageEffectClass->GetDefaultObject<UGameplayEffect>();
					auto Context = AbilitySystem->MakeEffectContext();
					Context.Get()->SetEffectCauser(this);
					AbilitySystem->ApplyGameplayEffectToTarget(DamageEffect, Victim->AbilitySystemComponent, 0, Context);
				}
			}
		}
	}
	Destroy();
}

