// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyPlayerInfo.h"

void ULobbyPlayerInfo::SetPlayerState(ADMLPlayerState* InPlayerState)
{
	PlayerState = InPlayerState;
	RefreshInfo();
}
