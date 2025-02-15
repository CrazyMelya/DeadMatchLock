// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagsSettings.h"
#include "DMLDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="DML Project Settings"))
class DEADMATCHLOCK_API UDMLDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	//This tag must be set in cooldown gameplay effect
	UPROPERTY(EditAnywhere, Config, Category="Cooldown")
	FGameplayTag CooldownTag;
};
