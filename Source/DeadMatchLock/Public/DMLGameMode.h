// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DMLPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "DMLGameMode.generated.h"

UCLASS(minimalapi)
class ADMLGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADMLGameMode();

	void NotifyPlayerDeath(ADMLPlayerController* Controller);

protected:
	virtual void RespawnPlayer(ADMLPlayerController* Controller);

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
	UPROPERTY()
	TMap<ADMLPlayerController*, FTimerHandle> RespawnTimers;
};



