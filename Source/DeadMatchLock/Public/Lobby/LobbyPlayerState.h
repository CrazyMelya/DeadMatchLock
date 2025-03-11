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

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	
public:
	UFUNCTION()
	void ToggleReadyState();

	UFUNCTION()
	void SetReadyState(bool bReady);

	UFUNCTION()
	void SetCharacterName(const FName& InCharacterName);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby State", ReplicatedUsing=OnRep_Ready)
	bool bReady = false;

	UFUNCTION()
	void OnRep_Ready();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby State", ReplicatedUsing=OnRep_CharacterName)
	FName CharacterName;

	UFUNCTION()
	void OnRep_CharacterName();

	FOnReadyStateChanged OnReadyStateChanged;
	FOnCharacterSelected OnCharacterSelected;
};
