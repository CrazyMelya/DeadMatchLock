// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerInfo.generated.h"

class ADMLPlayerState;
/**
 * 
 */
UCLASS()
class DEADMATCHLOCK_API ULobbyPlayerInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetPlayerState(ADMLPlayerState* InPlayerState);

	UFUNCTION(BlueprintImplementableEvent, Category = "LobbyPlayerInfo")
	void RefreshInfo();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LobbyPlayerInfo")
	ADMLPlayerState* PlayerState;
};
