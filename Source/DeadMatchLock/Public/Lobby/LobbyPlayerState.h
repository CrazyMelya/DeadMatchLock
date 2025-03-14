// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnReadyStateChanged, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterSelected, const FName&);

UCLASS()
class DEADMATCHLOCK_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void ToggleReadyState();

	UFUNCTION(NetMulticast, Reliable)
	void SetReadyState(bool InReady);

	UFUNCTION(NetMulticast, Reliable)
	void SetCharacterName(const FName& InCharacterName);

	UFUNCTION()
	bool CharacterSelected();

	UFUNCTION()
	bool GetReadyState();

	FOnReadyStateChanged OnReadyStateChanged;
	FOnCharacterSelected OnCharacterSelected;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CopyProperties(APlayerState* PlayerState) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby State", Replicated)
	bool bReady = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby State", Replicated)
	FName CharacterName;
};
