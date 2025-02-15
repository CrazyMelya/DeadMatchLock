// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLMenuPlayerController.h"

#include "DMLGameInstance.h"
#include "DMLPlayerState.h"
#include "OnlineSubsystemTypes.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

ADMLMenuPlayerController::ADMLMenuPlayerController(const FObjectInitializer& ObjectInitializer)
{
	SetShowMouseCursor(true);
}

void ADMLMenuPlayerController::BeginPlay()
{
	SetInputMode(FInputModeUIOnly());

	if (GetNetMode() == NM_Client || GetNetMode() == NM_Standalone)
	{
		MainMenuWidget = CreateWidget<UDMLBaseMainMenu>(this, MainMenuClass, FName("MainMenu"));
		if (MainMenuWidget)
			MainMenuWidget->AddToViewport();
	}
	Super::BeginPlay();
}

void ADMLMenuPlayerController::CreateLobby_Implementation(const FString& LobbyName)
{
	
	if (auto LobbyManager = ULobbyManager::Get(this))
	{
		FLobbyData LobbyData;
		auto bCreated = LobbyManager->CreateLobby(this, LobbyName, LobbyData);
		OnCreateLobbyCompleted(bCreated, LobbyData);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LobbyManager is null"));
	}
}

void ADMLMenuPlayerController::OnCreateLobbyCompleted_Implementation(bool Success, const FLobbyData& Lobby)
{
	OnCreateLobbyCompletedDelegate.Broadcast(Success, Lobby);
	OnCreateLobbyCompletedDelegate.Clear();
}

void ADMLMenuPlayerController::FindLobbies_Implementation(const FString& LobbyName)
{
	if (auto LobbyManager = ULobbyManager::Get(this))
	{
		TArray<FLobbyData> Lobbies;
		auto bFound = LobbyManager->FindLobbies(this, LobbyName, Lobbies);
		OnFindLobbiesCompleted(bFound, Lobbies);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LobbyManager is null"));
	}
}

void ADMLMenuPlayerController::OnFindLobbiesCompleted_Implementation(bool Success, const TArray<FLobbyData>& Lobbies)
{
	OnFindLobbiesCompletedDelegate.Broadcast(Success, Lobbies);
	OnFindLobbiesCompletedDelegate.Clear();
}

void ADMLMenuPlayerController::Login_Implementation(const FString& Nickname)
{
	if (auto DMLPlayerState = Cast<ADMLPlayerState>(PlayerState))
	{
		FUniqueNetIdStringRef FakeNetId = FUniqueNetIdString::Create(Nickname, FName(TEXT("NULL")));
		DMLPlayerState->SetUniqueId(*FakeNetId);
		DMLPlayerState->SetPlayerName(Nickname);
		DMLPlayerState->IsLogedIn = true;
		OnLoginCompleted(true);
	}
	else
	{
		OnLoginCompleted(false);
	}
}

void ADMLMenuPlayerController::OnLoginCompleted_Implementation(bool Success)
{
	OnLoginCompletedDelegate.Broadcast(Success);
	OnLoginCompletedDelegate.Clear();
}

void ADMLMenuPlayerController::JoinLobby_Implementation(const FLobbyData& LobbyData)
{
	if (auto LobbyManager = ULobbyManager::Get(this))
	{
		FLobbyData Lobby;
		auto bJoined = LobbyManager->JoinLobby(this, LobbyData.LobbyID, Lobby);
		OnJoinLobbyCompleted(bJoined, Lobby);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LobbyManager is null"));
	}
}

void ADMLMenuPlayerController::OnJoinLobbyCompleted_Implementation(bool Success, const FLobbyData& Lobby)
{
	OnJoinLobbyCompletedDelegate.Broadcast(Success, Lobby);
	OnJoinLobbyCompletedDelegate.Clear();
}

void ADMLMenuPlayerController::LeaveLobby_Implementation(const FLobbyData& LobbyData)
{
	if (auto LobbyManager = ULobbyManager::Get(this))
	{
		FLobbyData Lobby;
		auto bLeft = LobbyManager->LeaveLobby(this, LobbyData.LobbyID, Lobby);
		OnLeaveLobbyCompleted(bLeft);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LobbyManager is null"));
	}
}

void ADMLMenuPlayerController::OnLeaveLobbyCompleted_Implementation(bool Success)
{
	OnLeaveLobbyCompletedDelegate.Broadcast(Success);
	OnLeaveLobbyCompletedDelegate.Clear();
}

void ADMLMenuPlayerController::OnLobbyUpdated_Implementation(const FLobbyData& LobbyData)
{
	if (MainMenuWidget)
		MainMenuWidget->OnLobbyUpdated(LobbyData);
}

void ADMLMenuPlayerController::JoinServer_Implementation(const FString& URL)
{
	ClientTravel(URL, TRAVEL_Absolute, true);
}

void ADMLMenuPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateSetDelegate.Broadcast(Cast<ADMLPlayerState>(PlayerState.Get()));
}
