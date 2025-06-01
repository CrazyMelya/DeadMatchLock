// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "Engine/GameInstance.h"
#include "Templates/SharedPointer.h"
#include "DMLGameInstance.generated.h"

UCLASS()
class DEADMATCHLOCK_API UDMLGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()

public:
	void SetNumPlayers(int32 InNumPlayers) { NumPlayers = InNumPlayers; };
	
	int32 GetNumPlayers() const { return NumPlayers; };

	UFUNCTION(BlueprintCallable, Category = "DMLGameInstance")
	void SetNumKills(int32 InNumKills) { NumKills = InNumKills; };

	UFUNCTION(BlueprintCallable, Category = "DMLGameInstance")
	int32 GetNumKills() const { return NumKills; };

private:
	UPROPERTY()
	int32 NumPlayers = 0;

	UPROPERTY()
	int32 NumKills = 5;
};
