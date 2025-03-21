// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLPlayerState.h"
#include "Net/UnrealNetwork.h"

void ADMLPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	OnScoreChanged.Broadcast(GetScore());
}

void ADMLPlayerState::SetPlayerScore(float InScore)
{
	SetScore(InScore);
	OnRep_Score();
}

void ADMLPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, CharacterName);
}
