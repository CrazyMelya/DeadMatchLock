// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SharedTypes.h"
#include "GameFramework/PlayerState.h"
#include "DMLPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API ADMLPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Lobby State")
	bool bReady = false;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Character")
	FCharacterData CharacterData;

	UFUNCTION(NetMulticast, Reliable)
	void ToggleReadyState();

	UFUNCTION()
	void SetCharacterData(const FCharacterData& InCharacterData);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void CopyProperties(APlayerState* PlayerState) override;
};
