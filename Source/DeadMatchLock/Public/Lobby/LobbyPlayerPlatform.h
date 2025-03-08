// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SharedTypes.h"
#include "GameFramework/Actor.h"
#include "UI/LobbyPlayerInfo.h"
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
	void SetPlayer(ADMLPlayerState* InPlayerState);

	void RemovePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	FCharacterData CharacterData;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	USceneComponent* Root;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* PlatformMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
	USkeletalMeshComponent* CharacterMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
	UWidgetComponent* WC_LobbyPlayerInfo;

	UFUNCTION()
	bool GetPossessed();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(NetMulticast, Reliable)
	void RefreshInfo();

	UFUNCTION(NetMulticast, Reliable)
	void SetCharacterData(const FCharacterData& InCharacterData);

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnSetCharacterData")
	void BP_OnSetCharacterData(const FCharacterData& InCharacterData);

private:
	UPROPERTY(ReplicatedUsing=OnRep_PlayerState)
	ADMLPlayerState* PlayerState;

	UFUNCTION()
	void OnRep_PlayerState();

	UPROPERTY()
	ULobbyPlayerInfo* LobbyPlayerInfo;
};