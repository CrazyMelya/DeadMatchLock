// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "ClientPredictedActor.generated.h"

UCLASS()
class DEADMATCHLOCK_API AClientPredictedActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClientPredictedActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	uint32 ID;

	bool bIsPredictedCopy;

	UPROPERTY()
	AClientPredictedActor* FollowedServerActor;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
 
	static uint32 GenerateClientID(const UObject* WorldContext);

	void SetID(const uint32 NewID) { ID = NewID; };

	void SetIsPredictedCopy(bool bPredictedCopy) { bIsPredictedCopy = bPredictedCopy; };

	bool IsLocallyOwned() const;

	void LinkReplicatedWithPredicted(AClientPredictedActor* PredictedActor);

	UFUNCTION(BlueprintNativeEvent)
	void FollowReplicatedActor(AClientPredictedActor* PredictedActor);
	void FollowReplicatedActor_Implementation(AClientPredictedActor* PredictedActor);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateFromFollowedActor(AClientPredictedActor* FollowedActor, float DeltaTime);
	void UpdateFromFollowedActor_Implementation(AClientPredictedActor* FollowedActor, float DeltaTime);
};
