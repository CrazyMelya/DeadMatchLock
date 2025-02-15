// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyManager.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "JoinLobbyAsync.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJoinLobbyResultDelegate, const FLobbyData&, Lobby);

UCLASS()
class DEADMATCHLOCK_API UJoinLobbyAsync : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FJoinLobbyResultDelegate OnSuccess;
	
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;
	
	UFUNCTION(BlueprintCallable, Category = "Online | Lobby", DisplayName = "Join Lobby", meta=(BlueprintInternalUseOnly = "true"))
	static UJoinLobbyAsync* JoinLobbyAsync(UObject* WorldContextObject, APlayerController* PlayerController, const FLobbyData& LobbyData);

protected:
	virtual void Activate() override;

private:
	void OnJoinCompleted(bool Success, const FLobbyData& InLobbyData);
	
	// The player controller triggering things
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	UPROPERTY()
	UObject* WorldContextObject;

	FLobbyData LobbyData;
};
