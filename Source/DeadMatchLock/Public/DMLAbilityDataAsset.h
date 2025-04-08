// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SharedTypes.h"
#include "Engine/DataAsset.h"
#include "DMLAbilityDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UDMLAbilityDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<FAbilityInitData> Abilities;
	
	void GiveAbilities(UAbilitySystemComponent* ASC) const;
};
