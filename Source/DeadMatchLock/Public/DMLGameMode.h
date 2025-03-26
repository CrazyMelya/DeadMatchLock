// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayerController.h"
#include "GameFramework/GameMode.h"
#include "DMLGameMode.generated.h"

UCLASS(minimalapi)
class ADMLGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ADMLGameMode();

	void NotifyPlayerDeath(AGamePlayerController* DeadPlayer, AActor* Killer);

protected:
	virtual void RespawnPlayer(AGamePlayerController* Controller);
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void HandleSeamlessTravelPlayer(AController*& C) override;
	virtual void BeginPlay() override;
	virtual void InitGameState() override;
	virtual void HandleMatchHasEnded() override;
	virtual void HandleMatchHasStarted() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay")
	UDataTable* Characters;

	UPROPERTY()
	ADMLGameState* DMLGameState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay")
	int32 KillsToWin = 5;

private:
	UPROPERTY()
	TMap<AGamePlayerController*, FTimerHandle> RespawnTimers;

	UPROPERTY()
	FTimerHandle GameTimer;

	UFUNCTION()
	void StartMatchTimerTick();

	UFUNCTION()
	void GameTimerTick();

	UFUNCTION()
	void LeavingMapTimerTick();
};



