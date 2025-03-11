// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Lobby/LobbyGameMode.h"
#include "Lobby/LobbyGameState.h"
#include "BaseLobbyUI.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDynamicOnCharacterSelected, const FName&, CharacterName);

class ALobbyPlayerController;

UCLASS()
class DEADMATCHLOCK_API UBaseLobbyUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetPlayerController(ALobbyPlayerController* InPlayerController);
		
	UFUNCTION()
	void SetGameState(ALobbyGameState* InGameState);

	UPROPERTY(BlueprintCallable)
	FDynamicOnCharacterSelected OnCharacterSelected;

private:
	UPROPERTY()
	ALobbyPlayerController* PlayerController;

	UFUNCTION(BlueprintCallable)
	void ToggleReadyState();

	UFUNCTION(BlueprintCallable)
	void LeaveLobby();

	UFUNCTION(BlueprintCallable)
	void StartGame();
	
	UFUNCTION()
	void OnRep_GameState();

	FDelegateHandle OnAllReadyChangedDelegateHandle;
	FDelegateHandle OnLobbyStageChangedDelegateHandle;

protected:
	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnAvailableCharactersChanged")
	void BP_OnAvailableCharactersChanged(const TArray<FName>& InAvailableCharacters);

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnLobbyStageChanged")
	void BP_OnLobbyStageChanged(const TEnumAsByte<ELobbyStage>& LobbyStage);

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnAllReadyChanged")
	void BP_OnAllReadyChanged(bool bAllReady);
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FName> AvailableCharacters;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_GameState)
	ALobbyGameState* GameState;

	virtual void NativeDestruct() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};

