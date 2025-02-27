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
	void Possess(ALobbyPlayerController* InPlayerController);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	UWidgetComponent* LobbyPlayerInfo;

private:
	UPROPERTY()
	ALobbyPlayerController* PlayerController;
};
