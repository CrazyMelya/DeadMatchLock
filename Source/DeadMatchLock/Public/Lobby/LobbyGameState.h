// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyGameMode.h"
#include "GameFramework/GameState.h"
#include "LobbyGameState.generated.h"

UENUM(BlueprintType)
enum ELobbyStage
{
	Connection,
	Selection,
	FinalCountdown
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLobbyStageChanged, const TEnumAsByte<ELobbyStage>&)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAvailableCharactersChanged, const TArray<FName>&)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemainingTimeChanged, float)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAllReadyChanged, bool)

UCLASS()
class DEADMATCHLOCK_API ALobbyGameState : public AGameState
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetLobbyStage(TEnumAsByte<ELobbyStage> InLobbyStage);

	UFUNCTION()
	TEnumAsByte<ELobbyStage> GetLobbyStage();

	UFUNCTION()
	void SetAvailableCharacters(TArray<FName> InavailableCharacters);

	UFUNCTION()
	void SetAllReady(bool InAllReady);

	UFUNCTION()
	TArray<FName> GetAvailableCharacters();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_RemainingTime)
	float RemainingTime;
	
	FOnLobbyStageChanged OnLobbyStageChanged;
	FOnAvailableCharactersChanged OnAvailableCharactersChanged;
	FOnRemainingTimeChanged OnRemainingTimeChanged;
	FOnAllReadyChanged OnAllReadyChanged;

	UPROPERTY()
	ALobbyGameMode* GameMode;

	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_LobbyStage)
	TEnumAsByte<ELobbyStage> LobbyStage = Connection;
	
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_AllReady)
	bool bAllReady = false;
	
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_AvailableCharacters)
	TArray<FName> AvailableCharacters;

	UFUNCTION()
	virtual void OnRep_LobbyStage();
	
	UFUNCTION()
	virtual void OnRep_RemainingTime();
	
	UFUNCTION()
	virtual void OnRep_AvailableCharacters();
	
	UFUNCTION()
	virtual void OnRep_AllReady();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void AddPlayerState(APlayerState* PlayerState) override;

private:
	UFUNCTION()
	void OnPlayerToggleReady(bool bReady);
};
