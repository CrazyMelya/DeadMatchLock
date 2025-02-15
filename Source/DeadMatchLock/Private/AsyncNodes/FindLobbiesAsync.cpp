// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncNodes/FindLobbiesAsync.h"

#include "DMLMenuPlayerController.h"

UFindLobbiesAsync* UFindLobbiesAsync::FindLobbiesAsync(UObject* WorldContextObject, APlayerController* PlayerController, const FString& LobbyName)
{
	auto Action = NewObject<UFindLobbiesAsync>();
	
	Action->PlayerControllerWeakPtr = PlayerController;
	Action->WorldContextObject = WorldContextObject;
	Action->LobbyName = LobbyName;
	
	return Action;
}

void UFindLobbiesAsync::Activate()
{
	if (auto DMLController = Cast<ADMLMenuPlayerController>(PlayerControllerWeakPtr.Get()))
	{
		DMLController->OnFindLobbiesCompletedDelegate.AddUObject(this, &UFindLobbiesAsync::OnSearchCompleted);
		DMLController->FindLobbies(LobbyName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong controller class"));
		OnFailure.Broadcast();
	}
}

void UFindLobbiesAsync::OnSearchCompleted(bool Success, const TArray<FLobbyData>& Lobbies) const
{
	if (Success)
	{
		OnSuccess.Broadcast(Lobbies);
	}
	else
	{
		OnFailure.Broadcast();
	}
}
