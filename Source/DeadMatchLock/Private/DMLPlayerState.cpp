// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLPlayerState.h"
#include "Net/UnrealNetwork.h"

void ADMLPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	OnScoreChanged.Broadcast(GetScore());
}

void ADMLPlayerState::SetKills_Implementation(int32 NewKills)
{
	Kills = NewKills;
	OnRep_Kills();
}

void ADMLPlayerState::OnRep_Kills()
{
	OnKillsChanged.Broadcast(Kills);
}

void ADMLPlayerState::SetDeaths_Implementation(int32 NewDeaths)
{
	Deaths = NewDeaths;
	OnRep_Deaths();
}

void ADMLPlayerState::OnRep_Deaths()
{
	OnDeathsChanged.Broadcast(Deaths);
}

void ADMLPlayerState::SetPlayerScore_Implementation(float InScore)
{
	SetScore(InScore);
	OnRep_Score();
}

void ADMLPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY (ThisClass, CharacterName, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY (ThisClass, Kills, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY (ThisClass, Deaths, COND_None, REPNOTIFY_OnChanged);
}
