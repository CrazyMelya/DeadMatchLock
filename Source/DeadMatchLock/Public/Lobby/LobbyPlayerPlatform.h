// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LobbyPlayerPlatform.generated.h"

class UWidgetComponent;

UCLASS()
class DEADMATCHLOCK_API ALobbyPlayerPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALobbyPlayerPlatform();

	UFUNCTION()
	void SetPlayer(ALobbyPlayerState* InPlayerState);

	void RemovePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnSetPlayerState")
	void BP_OnSetPlayerState(ALobbyPlayerState* InPlayerState);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PlayerState)
	ALobbyPlayerState* PlayerState;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	USceneComponent* Root;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* PlatformMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UWidgetComponent* WCLobbyPlayerInfo;

	UFUNCTION()
	bool GetPossessed();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnReadyStateChanged")
	void BP_OnReadyStateChanged(bool bReady);
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnCharacterSelected")
	void BP_OnCharacterSelected(const FName& CharacterName);

private:
	UFUNCTION()
	void OnRep_PlayerState();
};