// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DMLFunctionLibrary.generated.h"

class UDMLGameInstance;
class ADMLGameMode;
class ADMLMainMenuGameMode;
class ALobbyGameMode;
/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UDMLFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static FVector RotateTowards(FVector From, FVector To, float MaxAngleDegrees = 180.0f);

	UFUNCTION(BlueprintPure, BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static ALobbyGameMode* GetLobbyGameMode(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static ADMLMainMenuGameMode* GetMainMenuGameMode(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static ADMLGameMode* GetDMLGameMode(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UDMLGameInstance* GetDMLGameInstance(const UObject* WorldContextObject);
};
