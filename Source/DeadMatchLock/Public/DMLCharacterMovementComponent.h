// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DMLCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DMLCharacterMovementComponent.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_Slide = 0,
};

UCLASS()
class DEADMATCHLOCK_API UDMLCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	virtual bool CanCrouchInCurrentState() const override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	void PhysSlide(float DeltaTime, int32 Iterations);
	
	bool bWantsSliding = false;

	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
public:
	FVector SlideInput;
	
	void StartSlide();

	void EndSlide();

	bool CanSlide();

	bool bIsSliding = false;
	
	FVector SlideDirection;
	
	float SlideSpeed = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float MinSlideActivationSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float MinSlideStopSpeed = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideDeceleration = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideControlFactor = 0.005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float MinSlideSlopeAngle = 10.0f;
};


class DEADMATCHLOCK_API FSavedMove_Slide : public FSavedMove_Character
{
public:
	FVector SavedSlideInput;

	virtual void Clear() override
	{
		FSavedMove_Character::Clear();
		SavedSlideInput = FVector::ZeroVector;
	}

	virtual uint8 GetCompressedFlags() const override
	{
		uint8 Result = FSavedMove_Character::GetCompressedFlags();
		// Добавляй флаги если нужно
		return Result;
	}

	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override
	{
		// Если SlideInput отличается — не комбайнируем
		if (SavedSlideInput != ((FSavedMove_Slide*)&NewMove)->SavedSlideInput)
			return false;

		return FSavedMove_Character::CanCombineWith(NewMove, Character, MaxDelta);
	}

	virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character & ClientData) override
	{
		FSavedMove_Character::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);
		
		if (const auto Char = Cast<ADMLCharacter>(Character))
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("SlideInput %f, %f"), SavedSlideInput.X, SavedSlideInput.Y));
			SavedSlideInput = Char->GetLastMovementInputVector().GetSafeNormal2D(); // Получаешь input направление
		}
	}

	virtual void PrepMoveFor(ACharacter* Character) override
	{
		FSavedMove_Character::PrepMoveFor(Character);

		if (auto* MoveComp = Cast<UDMLCharacterMovementComponent>(Character->GetCharacterMovement()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("SlideInput %f, %f"), SavedSlideInput.X, SavedSlideInput.Y));
			MoveComp->SlideInput = SavedSlideInput;
		}
	}
};

class FNetworkPredictionData_Client_Slide : public FNetworkPredictionData_Client_Character
{
public:
	FNetworkPredictionData_Client_Slide(const UCharacterMovementComponent& ClientMovement)
		: FNetworkPredictionData_Client_Character(ClientMovement)
	{
	}

	virtual FSavedMovePtr AllocateNewMove() override
	{
		return MakeShared<FSavedMove_Slide>();
	}
};
