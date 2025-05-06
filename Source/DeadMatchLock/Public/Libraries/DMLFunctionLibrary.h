// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DMLFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UDMLFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static FVector RotateTowards(FVector From, FVector To, float MaxAngleDegrees = 180.0f);
	
};
