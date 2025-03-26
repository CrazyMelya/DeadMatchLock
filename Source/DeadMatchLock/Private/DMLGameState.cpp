// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLGameState.h"

#include "DMLPlayerState.h"
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
			Controller->OnMatchStarted();
		}
	}
}

void ADMLGameState::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	
	for (auto Player : PlayerArray)
	{
		if (auto Controller = Cast<AGamePlayerController>(Player->GetOwner()))
		{
			Controller->OnMatchEnded();
		}
	}
}

void ADMLGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, GameTime);
}

void ADMLGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	if (auto DMLPlayerState =  Cast<ADMLPlayerState>(PlayerState))
	{
		DMLPlayerState->OnKillsChanged.AddDynamic(this, &ThisClass::OnPlayerKillsChanged);
		DMLPlayerArray.Add(DMLPlayerState);
	}
}

void ADMLGameState::OnRep_GameTime()
{
	OnGameTimeChanged.Broadcast(GameTime);
}

void ADMLGameState::OnPlayerKillsChanged(int32 NewKills)
{
	SortPlayerArray();
}

void ADMLGameState::SortPlayerArray()
{
	DMLPlayerArray.Sort([](const ADMLPlayerState& PlayerA, const ADMLPlayerState& PlayerB)
	{
		return PlayerA.Kills > PlayerB.Kills;
	});
	OnPlayersSorted.Broadcast();
}
