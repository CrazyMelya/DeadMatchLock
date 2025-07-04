// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DMLPlayerState.h"
#include "LobbyGameMode.h"
#include "LobbyPlayerPlatform.h"
#include "GameFramework/PlayerController.h"
#include "UI/BaseLobbyUI.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	UBaseLobbyUI* LobbyUI;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UBaseLobbyUI> LobbyUIClass;

	virtual void InitPlayerState() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	void SetPlayerPlatform(ALobbyPlayerPlatform* InPlayerPlatform);
	void RemovePlayerPlatform();
	
	UFUNCTION(Client, Reliable)
	void LeaveLobby();

	UFUNCTION(BlueprintImplementableEvent, DisplayName="LeaveLobby")
	void BP_LeaveLobby();

	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ToggleReadyState();
	
	UFUNCTION()
	void SetGameMode(ALobbyGameMode* InGameMode);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_LobbyPlayerState)
	ALobbyPlayerState* LobbyPlayerState;

	UFUNCTION()
	void OnRep_LobbyPlayerState();
	
	UFUNCTION(Server, Reliable)
	void StartGame();

private:
	UPROPERTY(Replicated)
	ALobbyPlayerPlatform* PlayerPlatform;
	
	UPROPERTY()
	ALobbyGameMode* LobbyGameMode;

	UFUNCTION(Server, Reliable)
	void PickCharacter(const FName& CharacterName);
	
};
