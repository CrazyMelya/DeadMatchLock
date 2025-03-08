// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLPlayerState.h"
#include "Net/UnrealNetwork.h"

void ADMLPlayerState::ToggleReadyState_Implementation()
{
	bReady = !bReady;
}

void ADMLPlayerState::SetCharacterData(const FCharacterData& InCharacterData)
{
	CharacterData = InCharacterData;
}

void ADMLPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bReady);
	DOREPLIFETIME(ThisClass, CharacterData);
}

void ADMLPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	UE_LOG(LogTemp, Log, TEXT("ADMLPlayerState::CopyProperties"));
	if (auto NewPS = Cast<ADMLPlayerState>(PlayerState))
	{
		NewPS->CharacterData = CharacterData;
	}
}
