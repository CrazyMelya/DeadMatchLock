// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerState.h"

#include "DMLPlayerState.h"
#include "Net/UnrealNetwork.h"


void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bReady);
	DOREPLIFETIME(ThisClass, CharacterName);
}

void ALobbyPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (auto NewPS = Cast<ADMLPlayerState>(PlayerState))
	{
		NewPS->CharacterName = CharacterName;
	}
}

void ALobbyPlayerState::ToggleReadyState()
{
	bReady = !bReady;
	OnRep_Ready();
}

void ALobbyPlayerState::SetReadyState(bool InReady)
{
	bReady = InReady;
	OnRep_Ready();
}

void ALobbyPlayerState::OnRep_Ready()
{
	OnReadyStateChanged.Broadcast(bReady);
}

void ALobbyPlayerState::OnRep_CharacterName()
{
	OnCharacterSelected.Broadcast(CharacterName);
}

void ALobbyPlayerState::SetCharacterName(const FName& InCharacterName)
{
	CharacterName = InCharacterName;
	OnRep_CharacterName();
}
