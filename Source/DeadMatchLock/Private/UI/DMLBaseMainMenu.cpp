// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DMLBaseMainMenu.h"

#include "Online.h"
#include "OnlineSessionSettings.h"

void UDMLBaseMainMenu::GetLobbyMembers(const FName lobbyName)
{
	if (auto Sessions = Online::GetSessionInterface())
	{
		if (auto Session = Sessions->GetNamedSession(lobbyName))
		{
			for (auto PlayerId : Session->RegisteredPlayers)
			{
				UE_LOG(LogTemp, Log, TEXT("Игрок в сессии: %s"), *PlayerId->ToString());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Session not found!"));
		}
	}
}
