// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GamePlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "DMLGameMode.generated.h"

UCLASS(minimalapi)
class ADMLGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADMLGameMode();

	void NotifyPlayerDeath(AGamePlayerController* Controller);

protected:
	virtual void RespawnPlayer(AGamePlayerController* Controller);

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay")
	UDataTable* Characters;

private:
	UPROPERTY()
	TMap<AGamePlayerController*, FTimerHandle> RespawnTimers;
};



