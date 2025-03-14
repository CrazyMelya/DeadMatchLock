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

void ALobbyPlayerState::ToggleReadyState_Implementation()
{
	SetReadyState(!bReady);
}

void ALobbyPlayerState::SetReadyState_Implementation(bool InReady)
{
	bReady = InReady;
	OnReadyStateChanged.Broadcast(bReady);
}

bool ALobbyPlayerState::CharacterSelected()
{
	return !CharacterName.IsNone();
}

bool ALobbyPlayerState::GetReadyState()
{
	return bReady;
}

void ALobbyPlayerState::SetCharacterName_Implementation(const FName& InCharacterName)
{
	CharacterName = InCharacterName;
	OnCharacterSelected.Broadcast(CharacterName);
}
