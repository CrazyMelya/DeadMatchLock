// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncNodes/CreateLobbyAsync.h"

#include "DMLGameInstance.h"
#include "DMLMenuPlayerController.h"

UCreateLobbyAsync* UCreateLobbyAsync::CreateLobbyAsync(UObject* WorldContextObject, APlayerController* PlayerController, const FString LobbyName)
{
	auto Action = NewObject<UCreateLobbyAsync>();
	Action->PlayerControllerWeakPtr = PlayerController;
	Action->WorldContextObject = WorldContextObject;
	Action->LobbyName = LobbyName;
	
	return Action;
}

void UCreateLobbyAsync::Activate()
{
	if (LobbyName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong lobby name"));
		OnFailure.Broadcast();
		return;
	}
	if (auto DMLController = Cast<ADMLMenuPlayerController>(PlayerControllerWeakPtr.Get()))
	{
		DMLController->OnCreateLobbyCompletedDelegate.AddUObject(this, &UCreateLobbyAsync::OnCreateCompleted);
		DMLController->CreateLobby(LobbyName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong controller class"));
		OnFailure.Broadcast();
	}
}

void UCreateLobbyAsync::OnCreateCompleted(bool Success, const FLobbyData& Lobby) const
{
	if (Success)
	{
		OnSuccess.Broadcast(Lobby);
	}
	else
	{
		OnFailure.Broadcast();
	}
}
