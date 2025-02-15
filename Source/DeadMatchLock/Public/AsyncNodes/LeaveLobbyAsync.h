// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyManager.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "LeaveLobbyAsync.generated.h"

/**
 * 
 */

UCLASS()
class DEADMATCHLOCK_API ULeaveLobbyAsync : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnSuccess;
	
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;
	
	UFUNCTION(BlueprintCallable, Category = "Online | Lobby", DisplayName = "Leave Lobby", meta=(BlueprintInternalUseOnly = "true"))
	static ULeaveLobbyAsync* LeaveLobbyAsync(UObject* WorldContextObject, APlayerController* PlayerController, const FLobbyData& LobbyData);

protected:
	virtual void Activate() override;

private:
	void OnLeaveCompleted(bool Success);
	
	// The player controller triggering things
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	UPROPERTY()
	UObject* WorldContextObject;

	FLobbyData LobbyData;
};
