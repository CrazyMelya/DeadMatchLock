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
	
public:
	UFUNCTION()
	void CloseLobby();

	UFUNCTION(Server, Reliable)
	void CheckAllReady();
	
protected:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;
	virtual void HandleSeamlessTravelPlayer(AController*& C) override;

	UPROPERTY(BlueprintReadOnly)
	TArray<ALobbyPlayerPlatform*> Platforms;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<ALobbyPlayerController*> Players;

	UPROPERTY(BlueprintReadOnly)
	ALobbyPlayerController* LobbyLeader;

private:
	UFUNCTION()
	void SetupFirstPlayers();

	UFUNCTION()
	void SetupPlatforms();

	UFUNCTION()
	void SetupNewPlayer(ALobbyPlayerController* Player);

	UFUNCTION()
	void OnNewPlayerConnected(ALobbyPlayerController* Player);

	UPROPERTY()
	bool bPlatformsSet = false;
};
