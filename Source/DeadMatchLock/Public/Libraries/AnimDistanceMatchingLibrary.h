// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnimDistanceMatchingLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UAnimDistanceMatchingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Distance Matching", meta=(BlueprintThreadSafe))
	static FSequenceEvaluatorReference AdvanceTimeByDistanceMatching(const FAnimUpdateContext& UpdateContext, const FSequenceEvaluatorReference& SequenceEvaluator,
		float DistanceTraveled, FName DistanceCurveName, FVector2D PlayRateClamp = FVector2D(0.75f, 1.25f));
	
	UFUNCTION(BlueprintCallable, Category = "Distance Matching", meta=(BlueprintThreadSafe))
	static FSequenceEvaluatorReference DistanceMatchToTarget(const FSequenceEvaluatorReference& SequenceEvaluator,
		float DistanceToTarget, FName DistanceCurveName);

	UFUNCTION(BlueprintCallable, Category = "Distance Matching", meta=(BlueprintThreadSafe))
	static FSequencePlayerReference SetPlayrateToMatchSpeed(const FSequencePlayerReference& SequencePlayer, 
		float SpeedToMatch, FVector2D PlayRateClamp = FVector2D(0.75f, 1.25f));
};
