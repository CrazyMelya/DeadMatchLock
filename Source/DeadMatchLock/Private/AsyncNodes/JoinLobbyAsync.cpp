// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncNodes/JoinLobbyAsync.h"

#include "DMLMenuPlayerController.h"

UJoinLobbyAsync* UJoinLobbyAsync::JoinLobbyAsync(UObject* WorldContextObject, APlayerController* PlayerController,
                                                 const FLobbyData& LobbyData)
{
	auto Action = NewObject<UJoinLobbyAsync>();
	
	Action->WorldContextObject = WorldContextObject;
	Action->PlayerControllerWeakPtr = PlayerController;
	Action->LobbyData = LobbyData;

	return Action;
}

void UJoinLobbyAsync::Activate()
{
	if (auto DMLController = Cast<ADMLMenuPlayerController>(PlayerControllerWeakPtr.Get()))
	{
		DMLController->OnJoinLobbyCompletedDelegate.AddUObject(this, &UJoinLobbyAsync::OnJoinCompleted);
		DMLController->JoinLobby(LobbyData);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong controller class"));
		OnFailure.Broadcast();
	}
}

void UJoinLobbyAsync::OnJoinCompleted(bool Success, const FLobbyData& InLobbyData)
{
	if (Success)
	{
		OnSuccess.Broadcast(InLobbyData);
	}
	else
	{
		OnFailure.Broadcast();
	}
}
