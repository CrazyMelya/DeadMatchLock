// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLMainMenuGameMode.h"

#include "DMLMenuPlayerController.h"
#include "DMLPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"


ADMLMainMenuGameMode::ADMLMainMenuGameMode(const FObjectInitializer& ObjectInitializer)
{
	PlayerControllerClass = ADMLMenuPlayerController::StaticClass();
	PlayerStateClass = ADMLPlayerState::StaticClass();
}

void ADMLMainMenuGameMode::GetSeamlessTravelActorList(bool bToTransition, TArray<AActor*>& ActorList)
{
	Super::GetSeamlessTravelActorList(bToTransition, ActorList);

	// for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	// {
	// 	APlayerController* PC = It->Get();
	// 	if (PC)
	// 	{
	// 		ActorList.Add(PC);
	// 	}
	// }
}

void ADMLMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!IsRunningDedicatedServer())
	{
		UE_LOG(LogTemp, Log, TEXT("Клиент, подключаетмя к мастер серверу..."));
		GetWorld()->GetFirstPlayerController()->ClientTravel("127.0.0.1:1", TRAVEL_Absolute, true);
	}
}

