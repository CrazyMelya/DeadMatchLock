// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DMLCharacter.h"
#include "InputAction.h"
#include "UI/DMLBaseHUD.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

/**
 * 
 */
class AClientPredictedActor;

struct FPredictedActorInfo
{
	/// The identifier. This is separate from the actor because we can create the ID first,
	/// then delay creating the client actor, so we need to know the *intended* ID
	uint32 ClientActorID = 0;

	/// The client predicted actor. Hopefully should be created before the server one replicates
	/// back to us, but in the case of a mis-prediction of lag, the server might send us the actor first
	TWeakObjectPtr<AClientPredictedActor> PredictedActor;

	/// The server replicated actor. 
	TWeakObjectPtr<AClientPredictedActor> ReplicatedActor;
};

UCLASS()
class DEADMATCHLOCK_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UDMLBaseHUD* HUD;

	UPROPERTY()
	UUserWidget* Menu;

	UFUNCTION(Client, Reliable)
	void OnMatchStarted();

	UFUNCTION(Client, Reliable)
	void OnMatchEnded();

	virtual void OnPossess(APawn* InPawn) override;

	uint32 RequestPredictedActorID();
	
	void SetPredictedActor(uint32 ID, AClientPredictedActor* PredictedActor);

	void SetPredictedActorReplicatedActor(uint32 ID, AClientPredictedActor* ReplicatedActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Interface")
	TSubclassOf<UDMLBaseHUD> HUDClass = UDMLBaseHUD::StaticClass();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "User Interface")
	TSubclassOf<UUserWidget> MenuClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC_Gameplay;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	UInputAction* IA_ToggleMenu;

	UFUNCTION(Client, Reliable)
	void CreateWidgets();

	UFUNCTION(Client, Reliable)
	void UpdateTargetCharacterToHUD();

	UFUNCTION()
	void ToggleMenu();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnRep_Pawn() override;

	uint32 NextPredictedActorID = 0;

	/// Client predicted actors that are owned locally, waiting for the server copy to match up with
	TArray<FPredictedActorInfo> PredictedActors;
};
