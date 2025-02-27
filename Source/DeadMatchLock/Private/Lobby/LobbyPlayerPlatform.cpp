// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerPlatform.h"

#include "Builders/CylinderBuilder.h"
#include "Components/Button.h"
#include "Components/WidgetComponent.h"


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
	CharacterMesh->SetHiddenInGame(true); // Show mesh when player connected
    CharacterMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
    CharacterMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	CharacterMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

    // Create and init widget components
	LobbyPlayerInfo = CreateDefaultSubobject<UWidgetComponent>(TEXT("LobbyPlayerInfo"));
	LobbyPlayerInfo->SetupAttachment(RootComponent);
	LobbyPlayerInfo->SetupAttachment(CharacterMesh);
	LobbyPlayerInfo->SetVisibility(false);
	LobbyPlayerInfo->SetWidgetSpace(EWidgetSpace::Screen);
	LobbyPlayerInfo->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
}

void ALobbyPlayerPlatform::Possess(ALobbyPlayerController* InPlayerController)
{
	if (PlayerController)
	{
		PlayerController = InPlayerController;
		
	}
}

// Called when the game starts or when spawned
void ALobbyPlayerPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALobbyPlayerPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

