// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "Engine/GameInstance.h"
#include "Templates/SharedPointer.h"
#include "DMLGameInstance.generated.h"

UCLASS()
class DEADMATCHLOCK_API UDMLGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetPlayersNum(int32 InPlayersNum) { PlayersNum = InPlayersNum; };
	
	UFUNCTION()
	int32 GetPlayersNum() const { return PlayersNum; };

private:
	UPROPERTY()
	int32 PlayersNum = 0;
};
