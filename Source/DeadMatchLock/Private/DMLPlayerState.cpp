// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLPlayerState.h"
#include "Net/UnrealNetwork.h"

void ADMLPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	OnScoreChanged.Broadcast(GetScore());
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("OnRep_Score"));
}

void ADMLPlayerState::SetPlayerScore(float InScore)
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("SetPlayerScore"));
	SetScore(InScore);
	OnRep_Score();
}

void ADMLPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, CharacterName);
}
