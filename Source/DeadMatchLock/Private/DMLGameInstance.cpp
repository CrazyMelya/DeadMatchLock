// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLGameInstance.h"

#include "OnlineSubsystemTypes.h"

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
