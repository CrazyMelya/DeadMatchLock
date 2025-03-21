// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLGameState.h"

#include "GamePlayerController.h"
#include "GameFramework/PlayerState.h"

void ADMLGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	for (auto Player : PlayerArray)
	{
		if (auto Controller = Cast<AGamePlayerController>(Player->GetOwner()))
		{
			Controller->RefreshGameStats();
		}
	}
}
