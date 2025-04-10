// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientPredictedActor.h"

#include "GamePlayerController.h"


// Sets default values
AClientPredictedActor::AClientPredictedActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AClientPredictedActor::BeginPlay()
{
	if (IsLocallyOwned() && GetWorld()->GetNetMode() == NM_Client)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("Bullet ID = %u"), ID));
		if (auto PC = Cast<AGamePlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld())))
		{
			// If either of these paths matches us up with our counterpart, will call LinkReplicatedWithPredicted
			if (bIsPredictedCopy)
			{
				// Register ourselves so we can match up
				PC->SetPredictedActor(ID, this);
			}
			else
			{
				// We're the server copy, having been replicated back to the owning client
				PC->SetPredictedActorReplicatedActor(ID, this);
			}
		}
	}

	// Important to do this last so that BP knows what happened
	Super::BeginPlay();
	
}

// Called every frame
void AClientPredictedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FollowedServerActor)
	{
		UpdateFromFollowedActor(FollowedServerActor, DeltaTime);
	}
}

uint32 AClientPredictedActor::GenerateClientID(const UObject* WorldContext)
{
	if (auto PC = Cast<AGamePlayerController>(GEngine->GetFirstLocalPlayerController(WorldContext->GetWorld())))
	{
		return PC->RequestPredictedActorID();
	}
	// Should never get here
	check(false)
	return 0;
}

bool AClientPredictedActor::IsLocallyOwned() const
{
	if (auto World = GetWorld())
	{
		if (auto PC = GEngine->GetFirstLocalPlayerController(World))
		{
			return IsOwnedBy(PC);
		}
	}
	return false;
}

void AClientPredictedActor::LinkReplicatedWithPredicted(AClientPredictedActor* PredictedActor)
{
	// We're the server copy, having been replicated back to the owning client
	// The client predicted version will be our visual
	// Hide ourselves
	SetActorHiddenInGame(true);
	// Also set the client version to not collide or react in any way, server will do this
	PredictedActor->SetActorEnableCollision(false);
	PredictedActor->FollowReplicatedActor(this);
}

void AClientPredictedActor::FollowReplicatedActor_Implementation(AClientPredictedActor* PredictedActor)
{
	FollowedServerActor = PredictedActor;
}

void AClientPredictedActor::UpdateFromFollowedActor_Implementation(AClientPredictedActor* FollowedActor, float DeltaTime)
{
	// Keep the visual actor pulled towards the replicated one
	SetActorLocation(
		FMath::VInterpTo(GetActorLocation(), FollowedActor->GetActorLocation(), DeltaTime, 10));

	SetActorRotation(
		FMath::RInterpTo(GetActorRotation(), FollowedActor->GetActorRotation(), DeltaTime, 10));
}

void AClientPredictedActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ID);
}

