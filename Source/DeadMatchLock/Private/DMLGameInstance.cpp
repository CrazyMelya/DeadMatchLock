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

void UDMLGameInstance::CreateSession(const FName SessionName)
{
	if (auto Sessions = Online::GetSessionInterface(GetWorld()))
	{
		FOnlineSessionSettings Settings;
		Settings.bIsDedicated = false;
		Settings.NumPublicConnections = 10;
		Settings.bAllowInvites = true;
		Settings.bShouldAdvertise = true;
		Sessions->OnCreateSessionCompleteDelegates.AddUObject(this, &UDMLGameInstance::OnCreateSession);
		Sessions->CreateSession(0, SessionName, Settings);
	}
}

void UDMLGameInstance::OnCreateSession(FName SessionName, bool Success)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->ConsoleCommand("open ?listen");
	}
	// if (auto Sessions = Online::GetSessionInterface(GetWorld()))
	// {
	// 	if (auto Session = Sessions->GetNamedSession(SessionName))
	// 	{
	// 		for (auto PlayerId : Session->RegisteredPlayers)
	// 		{
	// 			UE_LOG(LogTemp, Log, TEXT("Игрок в сессии: %s"), *PlayerId->ToString());
	// 		}
	// 	}
	// 	else
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Session not found!"));
	// 	}
	// }
}
