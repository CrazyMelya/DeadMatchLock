// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLGameState.h"

#include "GamePlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

void ADMLGameState::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	for (auto Player : PlayerArray)
	{
		if (auto Controller = Cast<AGamePlayerController>(Player->GetOwner()))
		{
			Controller->RefreshGameStats();
		}
	}
}

void ADMLGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, GameTime);
}

void ADMLGameState::OnRep_GameTime()
{
	OnGameTimeChanged.Broadcast(GameTime);
}
