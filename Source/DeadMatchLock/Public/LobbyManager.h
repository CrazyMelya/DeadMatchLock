// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SharedTypes.h"
#include "OnlineDelegateMacros.h"
#include "GameFramework/PlayerState.h"
#include "LobbyManager.generated.h"

using namespace UE::Online;

USTRUCT(BlueprintType, Blueprintable)
struct FLobbyMember
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	APlayerState* PlayerState;

	UPROPERTY(BlueprintReadOnly)
	bool bLeader;

	UPROPERTY(BlueprintReadOnly)
	bool bIsReady = false;


	FLobbyMember(APlayerState* PlayerState, bool bLeader = false): PlayerState(PlayerState), bLeader(bLeader) {};
	
	FLobbyMember(): PlayerState(nullptr), bLeader(false) {};

	bool operator==(const FLobbyMember& Other) const
	{
		return PlayerState->GetUniqueId() == Other.PlayerState->GetUniqueId();
	}
};

USTRUCT(BlueprintType, Blueprintable)
struct FLobbySettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bPublic;

	UPROPERTY(BlueprintReadOnly)
	int MaxPlayers;

	FLobbySettings(): bPublic(false), MaxPlayers(4) {}
};

USTRUCT(BlueprintType, Blueprintable)
struct FLobbyData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FGuid LobbyID;

	UPROPERTY(BlueprintReadOnly)
	TArray<FLobbyMember> Members;

	UPROPERTY(BlueprintReadOnly)
	FString LobbyName;

	UPROPERTY(BlueprintReadOnly)
	FLobbyMember LobbyLeader;

	UPROPERTY(BlueprintReadOnly)
	FLobbySettings Settings;
	
	FLobbyData(const FString& LobbyName, const FLobbyMember& LobbyLeader): LobbyName(LobbyName), LobbyLeader(LobbyLeader)
	{
		Members.Add(FLobbyMember(LobbyLeader));
	};

	bool CheckFreeSpace() const;
	
	FLobbyData(): LobbyName(FString()), LobbyLeader(FLobbyMember()) {};
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLobbyUpdated, const FLobbyData&)
DECLARE_MULTICAST_DELEGATE(FOnMatchStarted)

UCLASS(BlueprintType, Blueprintable)
class DEADMATCHLOCK_API ULobbyManager : public UObject
{
	GENERATED_BODY()

protected:
	
	TMap<FGuid, FLobbyData> Lobbies;
	
public:
	UFUNCTION(BlueprintCallable)
	bool CreateLobby(APlayerController* PlayerController, const FString& LobbyName, FLobbyData& Lobby);

	UFUNCTION(BlueprintCallable)
	bool JoinLobby(APlayerController* PlayerController, const FGuid& LobbyID, FLobbyData& Lobby);

	UFUNCTION(BlueprintCallable)
	bool FindLobbies(APlayerController* PlayerController, const FString& LobbyName, TArray<FLobbyData>& ResultLobbies);

	UFUNCTION(BlueprintCallable)
	bool LeaveLobby(APlayerController* PlayerController, const FGuid& LobbyID, FLobbyData& Lobby);

	UFUNCTION(BlueprintCallable)
	static ULobbyManager* Get(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	bool StartMatch(const FLobbyData& LobbyData);

	UFUNCTION(BlueprintCallable)
	void OnLobbyUpdated(const FLobbyData& LobbyData);

private:
	void CloseLobby(FGuid LobbyID);
};
