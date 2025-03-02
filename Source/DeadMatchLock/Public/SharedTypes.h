#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Blueprint/UserWidget.h"
#include "SharedTypes.generated.h"

class UUserWidget;

UENUM(Blueprinttype)
enum class EAbilityInputID : uint8
{
	None UMETA(DisplayName = "None"),
	Jump UMETA(DisplayName = "Jump"),
	Ability1 UMETA(DisplayName = "Ability1"),
	Ability2 UMETA(DisplayName = "Ability2"),
	Ability3 UMETA(DisplayName = "Ability3"),
	Ability4 UMETA(DisplayName = "Ability4"),
	Dash UMETA(DisplayName = "Dash"),
	Fire UMETA(DisplayName = "Fire"),
	Reload UMETA(DisplayName = "Reload") 
};

USTRUCT(BlueprintType)
struct FAbilityInitData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAbilityInputID InputID = EAbilityInputID::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* InputAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Bitmask, BitmaskEnum = "ETriggerEvent"))
	int32 ActivateTriggerEvent = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Bitmask, BitmaskEnum = "ETriggerEvent"))
	int32 DeactivateTriggerEvent = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> UserWidget;
};

USTRUCT(BlueprintType)
struct FCharactersData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAbilityInitData Ability1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAbilityInitData Ability2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAbilityInitData Ability3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAbilityInitData Ability4;
};

USTRUCT(BlueprintType)
struct FLobbyPlayerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bReady = false;
};

