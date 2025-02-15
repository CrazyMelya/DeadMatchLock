// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "FindLobbiesAsync.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFindLobbiesResultDelegate, const TArray<FLobbyData>&, Lobbies);

UCLASS()
class DEADMATCHLOCK_API UFindLobbiesAsync : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FFindLobbiesResultDelegate OnSuccess;
	
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;
	
	UFUNCTION(BlueprintCallable, Category = "Online | Lobby", DisplayName = "Find Lobbies", meta=(BlueprintInternalUseOnly = "true"))
	static UFindLobbiesAsync* FindLobbiesAsync(UObject* WorldContextObject, APlayerController* PlayerController, const FString& LobbyName);
	
	void OnSearchCompleted(bool Success, const TArray<FLobbyData>& Lobbies) const;
	
protected:
	virtual void Activate() override;

private:
	// The player controller triggering things
	TWeakObjectPtr<APlayerController> PlayerControllerWeakPtr;

	FString LobbyName;

	UPROPERTY()
	UObject* WorldContextObject;
};
