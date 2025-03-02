// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLPlayerState.h"
#include "Net/UnrealNetwork.h"

void ADMLPlayerState::ToggleReadyState_Implementation()
{
	bReady = !bReady;
}
