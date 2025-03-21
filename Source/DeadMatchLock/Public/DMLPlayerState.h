// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SharedTypes.h"
#include "GameFramework/PlayerState.h"
#include "DMLPlayerState.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, float, Score);

UCLASS()
class DEADMATCHLOCK_API ADMLPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Character")
	FName CharacterName;

	UPROPERTY(BlueprintAssignable)
	FOnScoreChanged OnScoreChanged;

	virtual void OnRep_Score() override;

	UFUNCTION(BlueprintCallable, DisplayName="Set Score")
	void SetPlayerScore(float InScore);
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
