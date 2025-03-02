// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyPlayerPlatform.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

	ALobbyGameMode(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TArray<ALobbyPlayerPlatform*> Platforms;

	UPROPERTY()
	TArray<ALobbyPlayerController*> Players;

	UPROPERTY()
	bool bPlatformsSet = false;

	UFUNCTION()
	void SetupFirstPlayers();

	UFUNCTION()
	void SetupPlatforms();

	UFUNCTION()
	void SetupNewPlayer(ALobbyPlayerController* Player);
};
