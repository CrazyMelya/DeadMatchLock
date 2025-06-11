// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseLobbyUI.h"

#include "Lobby/LobbyGameState.h"
#include "Lobby/LobbyPlayerController.h"
#include "Lobby/LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"

void UBaseLobbyUI::SetGameState(ALobbyGameState* InGameState)
{
	GameState = InGameState;
	OnRep_GameState();
}

void UBaseLobbyUI::BindOnCharacterSelected(ALobbyPlayerState* PlayerState)
{
	if (PlayerState)
		PlayerState->OnCharacterSelected.AddUObject(this, &ThisClass::BP_OnCharacterSelected);
}

void UBaseLobbyUI::ToggleReadyState()
{
	PlayerController->ToggleReadyState();
}

void UBaseLobbyUI::LeaveLobby()
{
	PlayerController->LeaveLobby();
}

void UBaseLobbyUI::StartGame()
{
	PlayerController->StartGame();
}

void UBaseLobbyUI::OnRep_GameState()
{
	OnAllReadyChangedDelegateHandle = GameState->OnAvailableCharactersChanged.AddUObject(this, &ThisClass::BP_OnAvailableCharactersChanged);
	OnLobbyStageChangedDelegateHandle = GameState->OnLobbyStageChanged.AddUObject(this, &ThisClass::BP_OnLobbyStageChanged);
	OnAllReadyChangedDelegateHandle = GameState->OnAllReadyChanged.AddUObject(this, &ThisClass::BP_OnAllReadyChanged);
	AvailableCharacters = GameState->GetAvailableCharacters();
}

void UBaseLobbyUI::SetPlayerController(ALobbyPlayerController* InPlayerController)
{
	PlayerController = InPlayerController;
}

void UBaseLobbyUI::NativeDestruct()
{
	Super::NativeDestruct();

	GameState->OnAllReadyChanged.Remove(OnAllReadyChangedDelegateHandle);
	GameState->OnLobbyStageChanged.Remove(OnAllReadyChangedDelegateHandle);
	if (OnAllReadyChangedDelegateHandle.IsValid())
		GameState->OnAvailableCharactersChanged.Remove(OnAllReadyChangedDelegateHandle);
}

void UBaseLobbyUI::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, GameState);
}
