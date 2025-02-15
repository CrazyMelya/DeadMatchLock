// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyManager.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "CreateLobbyAsync.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateLobbyResultDelegate, const FLobbyData&, Lobby);

UCLASS()
class DEADMATCHLOCK_API UCreateLobbyAsync : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

public:
	// Called when the lobby was created successfully
	UPROPERTY(BlueprintAssignable)
	FCreateLobbyResultDelegate OnSuccess;

	// Called when there was an error creating the lobby
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;
	
	UFUNCTION(BlueprintCallable, Category = "Online | Lobby", DisplayName = "Create Lobby", meta=(BlueprintInternalUseOnly = "true"))
	static UCreateLobbyAsync* CreateLobbyAsync(UObject* WorldContextObject, APlayerController* PlayerController, const FString LobbyName);

protected:
	virtual void Activate() override;
	
private:
	void OnCreateCompleted(bool Success, const FLobbyData& Lobby) const;
	
	// The player controller triggering things
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	UPROPERTY()
	UObject* WorldContextObject;

	FString LobbyName;
};
