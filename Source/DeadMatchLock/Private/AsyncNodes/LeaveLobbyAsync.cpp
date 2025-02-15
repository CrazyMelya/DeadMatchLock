// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncNodes/LeaveLobbyAsync.h"

#include "DMLMenuPlayerController.h"

ULeaveLobbyAsync* ULeaveLobbyAsync::LeaveLobbyAsync(UObject* WorldContextObject, APlayerController* PlayerController,
                                                    const FLobbyData& LobbyData)
{
	auto Action = NewObject<ULeaveLobbyAsync>();

	Action->PlayerControllerWeakPtr = PlayerController;
	Action->WorldContextObject = WorldContextObject;
	Action->LobbyData = LobbyData;

	return Action;
}

void ULeaveLobbyAsync::Activate()
{
	if (auto DMLController = Cast<ADMLMenuPlayerController>(PlayerControllerWeakPtr.Get()))
	{
		DMLController->OnLeaveLobbyCompletedDelegate.AddUObject(this, &ULeaveLobbyAsync::OnLeaveCompleted);
		DMLController->LeaveLobby(LobbyData);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong controller class"));
		OnFailure.Broadcast();
	}
}

void ULeaveLobbyAsync::OnLeaveCompleted(bool Success)
{
	if (Success)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
}
