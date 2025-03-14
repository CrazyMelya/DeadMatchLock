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

	UFUNCTION()
	void PickCharacter(ALobbyPlayerState* PickingPlayer, const FName& CharacterName);

	UFUNCTION()
	void PickRandomCharacter(ALobbyPlayerState* PickingPlayer);

	UPROPERTY()
	class ALobbyGameState* LobbyGameState;

	UFUNCTION()
	void StartSelectionStage();
	
protected:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;
	virtual void HandleSeamlessTravelPlayer(AController*& C) override;
	virtual void InitGameState() override;

	UPROPERTY(BlueprintReadOnly)
	TArray<ALobbyPlayerPlatform*> Platforms;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<ALobbyPlayerController*> Players;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDataTable* CharactersDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SelectionTime = 30;

	UFUNCTION(BlueprintImplementableEvent, DisplayName="StartGame")
	void BP_StartGame();

private:
	UFUNCTION()
	void SetupFirstPlayers();

	UFUNCTION()
	void SetupPlatforms();

	UFUNCTION()
	void SetupCharacters();

	UFUNCTION()
	void SetupNewPlayer(ALobbyPlayerController* NewPlayer);

	UFUNCTION()
	void OnNewPlayerConnected(ALobbyPlayerController* NewPlayer);

	UPROPERTY()
	bool bPlatformsSet = false;

	UPROPERTY()
	TArray<FName> AvailableCharacters;

	UPROPERTY()
	FTimerHandle Timer;

	UFUNCTION()
	void SelectCharacterTimerTick();

	UFUNCTION()
	void EndSelectionStage();

	UFUNCTION()
	void PickRandomCharacters();
	
	UFUNCTION()
	void FinalCountdownTick();
};
