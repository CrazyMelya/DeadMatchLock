// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystemTypes.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"

void UDMLGameInstance::Init()
{
	Super::Init();

	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		LobbyManager = NewObject<ULobbyManager>(this);
	}
}

ULobbyManager* UDMLGameInstance::GetLobbyManager() const
{
	return LobbyManager;
}

void UDMLGameInstance::CreateDedicatedSession()
{
	if (auto OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		if (auto Sessions = OnlineSubsystem->GetSessionInterface())
		{
			FOnlineSessionSettings Settings;
			Settings.bIsDedicated = true;
			Settings.NumPublicConnections = 10;
			Sessions->CreateSession(0, FName("TestSession"), Settings);
		}
	}
}
