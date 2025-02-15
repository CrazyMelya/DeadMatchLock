// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyManager.h"
#include "SharedTypes.h"
#include "Engine/GameInstance.h"
#include "Templates/SharedPointer.h"
#include "DMLGameInstance.generated.h"

UCLASS()
class DEADMATCHLOCK_API UDMLGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	virtual void Init() override;

	UPROPERTY()
	ULobbyManager* LobbyManager;

public:
	UFUNCTION(BlueprintCallable)
	ULobbyManager* GetLobbyManager() const;
};
