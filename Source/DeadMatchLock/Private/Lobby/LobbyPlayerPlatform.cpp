// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerPlatform.h"

#include "Components/WidgetComponent.h"
#include "Lobby/LobbyPlayerController.h"
#include "Lobby/LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ALobbyPlayerPlatform::ALobbyPlayerPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	// Create and init platform mesh
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylinderMeshAsset.Succeeded())
		PlatformMesh->SetStaticMesh(CylinderMeshAsset.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (MaterialAsset.Succeeded())
		PlatformMesh->SetMaterial(0, MaterialAsset.Object);
	PlatformMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 15.0f));
	PlatformMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 0.3f));

	// Create and init base character mesh
	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetupAttachment(RootComponent);
	CharacterMesh->SetHiddenInGame(true, true); // Show mesh when player connect
    CharacterMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
    CharacterMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	CharacterMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

    // Create and init widget components
	WCLobbyPlayerInfo = CreateDefaultSubobject<UWidgetComponent>(TEXT("LobbyPlayerInfo"));
	WCLobbyPlayerInfo->SetupAttachment(RootComponent);
	WCLobbyPlayerInfo->SetupAttachment(CharacterMesh);
	WCLobbyPlayerInfo->SetHiddenInGame(true);
	WCLobbyPlayerInfo->SetWidgetSpace(EWidgetSpace::Screen);
	WCLobbyPlayerInfo->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));

	SetReplicates(true);
}

void ALobbyPlayerPlatform::SetPlayer(ALobbyPlayerState* InPlayerState)
{
	PlayerState = InPlayerState;
	OnRep_PlayerState();
}

void ALobbyPlayerPlatform::RemovePlayer()
{
	PlayerState = nullptr;
	
	OnRep_PlayerState();
}

// Called when the game starts or when spawned
void ALobbyPlayerPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	if (PlayerState)
	{
		PlayerState->OnReadyStateChanged.AddUObject(this, &ALobbyPlayerPlatform::BP_OnReadyStateChanged);
		PlayerState->OnCharacterSelected.AddUObject(this, &ALobbyPlayerPlatform::BP_OnCharacterSelected);
		CharacterMesh->SetHiddenInGame(false, true);
	}
}

// Called every frame
void ALobbyPlayerPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ALobbyPlayerPlatform::GetPossessed()
{
	return PlayerState != nullptr;
}

void ALobbyPlayerPlatform::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerPlatform, PlayerState);
}

void ALobbyPlayerPlatform::OnRep_PlayerState()
{
	if (PlayerState)
	{
		PlayerState->OnReadyStateChanged.AddUObject(this, &ALobbyPlayerPlatform::BP_OnReadyStateChanged);
		PlayerState->OnCharacterSelected.AddUObject(this, &ALobbyPlayerPlatform::BP_OnCharacterSelected);
		CharacterMesh->SetHiddenInGame(false, true);
		BP_OnSetPlayerState(PlayerState);
	}
	else
	{
		CharacterMesh->SetHiddenInGame(true, true);
	}
}

