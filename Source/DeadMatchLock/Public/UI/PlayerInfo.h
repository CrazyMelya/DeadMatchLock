// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API UPlayerInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetHealth(float NewHealth);

	UFUNCTION()
	void SetMaxHealth(float NewMaxHealth);
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "PlayerInfo")
	float Health;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "PlayerInfo")
	float MaxHealth;
	
};
