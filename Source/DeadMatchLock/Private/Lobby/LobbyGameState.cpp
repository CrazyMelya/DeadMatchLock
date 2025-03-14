// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyGameState.h"

#include "Lobby/LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"


void ALobbyGameState::SetLobbyStage(TEnumAsByte<ELobbyStage> InLobbyStage)
{
	LobbyStage = InLobbyStage;
	OnRep_LobbyStage();
}

TEnumAsByte<ELobbyStage> ALobbyGameState::GetLobbyStage()
{
	return LobbyStage;
}

void ALobbyGameState::SetAvailableCharacters(TArray<FName> InavailableCharacters)
{
	AvailableCharacters = InavailableCharacters;
	OnRep_AvailableCharacters();
}

void ALobbyGameState::SetAllReady(bool InAllReady)
{
	bAllReady = InAllReady;
	OnRep_AllReady();
}

TArray<FName> ALobbyGameState::GetAvailableCharacters()
{
	return AvailableCharacters;
}

void ALobbyGameState::OnRep_LobbyStage()
{
	OnLobbyStageChanged.Broadcast(LobbyStage);
}

void ALobbyGameState::OnRep_RemainingTime()
{
	OnRemainingTimeChanged.Broadcast(RemainingTime);
}

void ALobbyGameState::OnRep_AvailableCharacters()
{
	OnAvailableCharactersChanged.Broadcast(AvailableCharacters);
}

void ALobbyGameState::OnRep_AllReady()
{
	OnAllReadyChanged.Broadcast(bAllReady);
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, LobbyStage);
	DOREPLIFETIME(ThisClass, RemainingTime);
	DOREPLIFETIME(ThisClass, AvailableCharacters);
	DOREPLIFETIME(ThisClass, bAllReady);
}

void ALobbyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	if (auto LobbyPlayerState = Cast<ALobbyPlayerState>(PlayerState))
	{
		LobbyPlayerState->OnReadyStateChanged.AddUObject(this, &ALobbyGameState::OnPlayerToggleReady);
	}
}

void ALobbyGameState::OnPlayerToggleReady(bool bReady)
{
	bool bAllPlayersReady = true;
	for (auto Player : PlayerArray)
	{
		auto LobbyPlayerState = Cast<ALobbyPlayerState>(Player);
		if (LobbyPlayerState && !LobbyPlayerState->GetReadyState())
		{
			bAllPlayersReady = false;
			break;
		}
	}
	SetAllReady(bAllPlayersReady);
}
