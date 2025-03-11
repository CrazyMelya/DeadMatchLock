// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyManager.h"

#include "DMLGameInstance.h"
#include "DMLMenuPlayerController.h"
#include "DMLPlayerState.h"
#include "Kismet/GameplayStatics.h"


bool FLobbyData::CheckFreeSpace() const
{
	return Members.Num() < Settings.MaxPlayers;
}

bool ULobbyManager::CreateLobby(APlayerController* PlayerController, const FString& LobbyName, FLobbyData& Lobby)
{
	auto DMLMenuController = Cast<ADMLMenuPlayerController>(PlayerController);
	
	if (!DMLMenuController) return false;
	
	if (auto PlayerState = DMLMenuController->GetPlayerState<ADMLPlayerState>())
	{
		auto LobbyLeader = FLobbyMember(PlayerState, true);
		Lobby = FLobbyData(LobbyName, LobbyLeader);
		auto LobbyID = FGuid::NewGuid();
		Lobby.LobbyID = LobbyID;
		Lobbies.Add(LobbyID, Lobby);
		auto OnLobbyUpdated = FOnLobbyUpdated();
		return true;
	}
	return false;
}

bool ULobbyManager::JoinLobby(APlayerController* PlayerController, const FGuid& LobbyID, FLobbyData& Lobby)
{
	auto DMLMenuController = Cast<ADMLMenuPlayerController>(PlayerController);

	if (!DMLMenuController) return false;

	auto PlayerState = DMLMenuController->GetPlayerState<ADMLPlayerState>();

	if (!PlayerState) return false;
	
	auto FoundLobby = Lobbies.Find(LobbyID);
	if (FoundLobby && FoundLobby->CheckFreeSpace())
	{
		FoundLobby->Members.Add(FLobbyMember(PlayerState));
		Lobby = *FoundLobby;
		OnLobbyUpdated(Lobby);
		return true;
	}
	return false;
}

bool ULobbyManager::FindLobbies(APlayerController* PlayerController, const FString& LobbyName, TArray<FLobbyData>& ResultLobbies)
{
	bool Result = false;
	ResultLobbies.Empty();
	for (auto Lobby : Lobbies)
	{
		if (Lobby.Value.LobbyName.Contains(LobbyName))
		{
			ResultLobbies.Add(Lobby.Value);
			Result = true;
		}
	}
	return Result;
}

bool ULobbyManager::LeaveLobby(APlayerController* PlayerController, const FGuid& LobbyID, FLobbyData& Lobby)
{
	bool Result = false;
	auto DMLMenuController = Cast<ADMLMenuPlayerController>(PlayerController);

	if (!DMLMenuController) return false;

	auto PlayerState = DMLMenuController->GetPlayerState<ADMLPlayerState>();

	if (!PlayerState) return false;
	
	if (auto FoundLobby = Lobbies.Find(LobbyID))
	{
		for (auto LobbyMember : FoundLobby->Members)
		{
			if (LobbyMember.PlayerState == PlayerState)
			{
				FoundLobby->Members.Remove(LobbyMember);
				Result = true;
				if (FoundLobby->LobbyLeader.PlayerState == PlayerState && FoundLobby->Members.Num() > 0)
				{
					FoundLobby->Members[0].bLeader = true;
					FoundLobby->LobbyLeader = FoundLobby->Members[0];
				}
				Lobby = *FoundLobby;
				OnLobbyUpdated(Lobby);
				break;
			}
		}
		if (FoundLobby->Members.Num() == 0)
			CloseLobby(LobbyID);
	}
	return Result;
}

ULobbyManager* ULobbyManager::Get(const UObject* WorldContextObject)
{
	const auto GameInstance = Cast<UDMLGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));

	if (!GameInstance) return nullptr;

	return nullptr;
}

bool ULobbyManager::StartMatch(const FLobbyData& LobbyData)
{
	auto FoundLobby = Lobbies.Find(LobbyData.LobbyID);
	
	if (!FoundLobby) return false;

	for (auto Member : FoundLobby->Members)
	{
		auto DMLController = Cast<ADMLMenuPlayerController>(Member.PlayerState->GetPlayerController());

		DMLController->JoinServer("127.0.0.1:7778");
	}
	return true;
}

void ULobbyManager::CloseLobby(FGuid LobbyID)
{
	Lobbies.Remove(LobbyID);
}

void ULobbyManager::OnLobbyUpdated(const FLobbyData& LobbyData)
{
	for (auto Member : LobbyData.Members)
	{
		if (auto DMLMenuController = Cast<ADMLMenuPlayerController>(Member.PlayerState->GetPlayerController()))
		{
			DMLMenuController->OnLobbyUpdated(LobbyData);
		}
	}
}

