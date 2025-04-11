#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Blueprint/UserWidget.h"
#include "SharedTypes.generated.h"

#define TAG_STUNNED "Status.Stunned"
#define TAG_NEEDRELOAD "Status.NeedReload"
#define TAG_RELOAD "Ability.Channeling.Reload"
#define TAG_WINDOW_ROLLJUMP "Status.Window.RollJump"

class UUserWidget;

UENUM(Blueprinttype)
enum class EAbilityInputID : uint8
{
	None UMETA(DisplayName = "None"),
	Jump UMETA(DisplayName = "Jump"),
	AirJump UMETA(DisplayName = "AirJump"),
	RollJump UMETA(DisplayName = "RollJump"),
	WallJump UMETA(DisplayName = "WallJump"),
	Ability1 UMETA(DisplayName = "Ability1"),
	Ability2 UMETA(DisplayName = "Ability2"),
	Ability3 UMETA(DisplayName = "Ability3"),
	Ability4 UMETA(DisplayName = "Ability4"),
	Dash UMETA(DisplayName = "Dash"),
	AirDash UMETA(DisplayName = "AirDash"),
	Fire UMETA(DisplayName = "Fire"),
	Reload UMETA(DisplayName = "Reload"),
	Move UMETA(DisplayName = "Move"),
	Look UMETA(DisplayName = "Look"),
	HeavyStrike UMETA(DisplayName = "HeavyStrike"),
	LightStrike UMETA(DisplayName = "LightStrike"),
	Parry UMETA(DisplayName = "Parry"),
	Climb UMETA(DisplayName = "Climb"),
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
struct FCharacterData: public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* FirstMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* SecondMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Avatar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ADMLCharacter> CharacterClass = nullptr;
};


struct FSavedFrame
{
	FVector Location;
	float Timestamp;
};

