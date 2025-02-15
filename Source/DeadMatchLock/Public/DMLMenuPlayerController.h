// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyManager.h"
#include "GameFramework/PlayerController.h"
#include "UI/DMLBaseMainMenu.h"
#include "DMLMenuPlayerController.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_TwoParams(FBaseLobbyDelegate, bool, const FLobbyData&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFindLobbiesCompleted, bool, const TArray<FLobbyData>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLeaveLobbyCompeleted, bool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginCompleted, bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateSeted, ADMLPlayerState*, PlayerState);

UCLASS()
class DEADMATCHLOCK_API ADMLMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADMLMenuPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	FBaseLobbyDelegate OnCreateLobbyCompletedDelegate;
	FOnFindLobbiesCompleted OnFindLobbiesCompletedDelegate;
	FBaseLobbyDelegate OnJoinLobbyCompletedDelegate;
	FOnLeaveLobbyCompeleted OnLeaveLobbyCompletedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnLoginCompleted OnLoginCompletedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateSeted OnPlayerStateSetDelegate;
	
	UFUNCTION(Server, Reliable)
	void CreateLobby(const FString& LobbyName);
	
	UFUNCTION(Server, Reliable)
	void FindLobbies(const FString& LobbyName);

	UFUNCTION(Server, Reliable)
	void JoinLobby(const FLobbyData& LobbyData);

	UFUNCTION(Server, Reliable)
	void LeaveLobby(const FLobbyData& LobbyData);
	
	UFUNCTION(Server, Reliable, BlueprintCallable, DisplayName = "Login")
	void Login(const FString& Nickname);
	
	UFUNCTION(Client, Reliable)
	void OnLobbyUpdated(const FLobbyData& LobbyData);

	UFUNCTION(Client, Reliable)
	void JoinServer(const FString& URL);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UDMLBaseMainMenu> MainMenuClass;

	UPROPERTY(BlueprintReadWrite, Category = "Widget")
	UDMLBaseMainMenu* MainMenuWidget;

private:
	UFUNCTION(Client, Reliable)
	void OnCreateLobbyCompleted(bool Success, const FLobbyData& Lobby);
	
	UFUNCTION(Client, Reliable)
	void OnJoinLobbyCompleted(bool Success, const FLobbyData& Lobby);
	
	UFUNCTION(Client, Reliable)
	void OnFindLobbiesCompleted(bool Success, const TArray<FLobbyData>& Lobbies);
	
	UFUNCTION(Client, Reliable)
	void OnLeaveLobbyCompleted(bool Success);
	
	UFUNCTION(Client, Reliable)
	void OnLoginCompleted(bool Success);

	virtual void OnRep_PlayerState() override;
	
};

