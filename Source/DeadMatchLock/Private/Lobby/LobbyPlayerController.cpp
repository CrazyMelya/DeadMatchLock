// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerController.h"

#include "Online.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


void ALobbyPlayerController::InitPlayerState()
{
	Super::InitPlayerState();

	DMLPlayerState = Cast<ADMLPlayerState>(PlayerState);
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, DMLPlayerState);
	DOREPLIFETIME(ALobbyPlayerController, PlayerPlatform);
}

void ALobbyPlayerController::SetPlayerPlatform(ALobbyPlayerPlatform* InPlayerPlatform)
{
	PlayerPlatform = InPlayerPlatform;
	PlayerPlatform->SetPlayer(DMLPlayerState);
}

void ALobbyPlayerController::RemovePlayerPlatform()
{
	PlayerPlatform->RemovePlayer();
	PlayerPlatform = nullptr;
}

void ALobbyPlayerController::LeaveLobby()
{
	if (HasAuthority())
	{
		
	}
	if (auto Sessions = Online::GetSessionInterface())
	{
		Sessions->DestroySession(NAME_GameSession);
		Sessions->OnDestroySessionCompleteDelegates.AddUObject(this, &ALobbyPlayerController::OnSessionDestroyed);
	}
}

void ALobbyPlayerController::OnSessionDestroyed(FName SessionName, bool bSuccess)
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/MainMenu"));
	// if (HasAuthority()) // Если это хост (Listen Server)
	// {
	// 	GetWorld()->ServerTravel(TEXT("/Game/Maps/MainMenu")); // Закрываем сервер
	// }
	// else // Если это клиент
	// {
	// 	ClientTravel(TEXT("/Game/Maps/MainMenu"), TRAVEL_Absolute);
	// }
}

void ALobbyPlayerController::BeginPlay()
{
	SetInputMode(FInputModeUIOnly());
	
	if (IsLocalController() && !LobbyUI)
	{
		LobbyUI = CreateWidget<UBaseLobbyUI>(this, LobbyUIClass, FName("LobbyUI"));
		if (LobbyUI)
		{
			LobbyUI->SetPlayerController(this);
			LobbyUI->AddToViewport();
		}
	}

	Super::BeginPlay();
}

void ALobbyPlayerController::SetGameMode(ALobbyGameMode* InGameMode)
{
	GameMode = InGameMode;
}

void ALobbyPlayerController::ToggleReadyState_Implementation()
{
	if (DMLPlayerState && PlayerPlatform)
	{
		DMLPlayerState->ToggleReadyState();
		PlayerPlatform->RefreshInfo();
	}
}
