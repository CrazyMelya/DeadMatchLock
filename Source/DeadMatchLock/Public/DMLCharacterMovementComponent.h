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

USTRUCT(BlueprintType)
struct FDMLCharacterGroundInfo
{
	GENERATED_BODY()

	FDMLCharacterGroundInfo()
		: LastUpdateFrame(0)
		, GroundDistance(0.0f)
	{}

	uint64 LastUpdateFrame;

	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;
};

UCLASS()
class DEADMATCHLOCK_API UDMLCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	class DEADMATCHLOCK_API FDMLSavedMove_Character : public FSavedMove_Character
	{
	public:
		uint8 bSavedWantsToSlide : 1;
		

		virtual void Clear() override
		{
			FSavedMove_Character::Clear();
			bSavedWantsToSlide = false;
		}

		virtual uint8 GetCompressedFlags() const override
		{
			uint8 Result = FSavedMove_Character::GetCompressedFlags();
			if (bSavedWantsToSlide)
			{
				Result |= FLAG_Custom_0;
			}
			return Result;
		}

		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character & ClientData) override
		{
			FSavedMove_Character::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);
			
			bSavedWantsToSlide = Cast<ADMLCharacter>(Character)->bWantsToSlide;
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
			return MakeShared<FDMLSavedMove_Character>();
		}
	};
	
	virtual bool CanCrouchInCurrentState() const override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	void PhysSlide(float DeltaTime, int32 Iterations);
	
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
	bool bIsSliding = false;

	float FloorSlopeAngle;

public:
	bool bWantsToSlide = false;
	
	void StartSlide();

	void EndSlide();

	bool CanSlide();

	UFUNCTION(BlueprintPure, Category = "Slide")
	bool GetIsSliding() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float MinSlideActivationSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float MinSlideStopSpeed = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideDeceleration = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideControlFactor = 0.5f;
	
	// Min slope angle that allows slide
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float MinSlideSlopeAngle = 10.0f;

	// Max angle between slope direction and velocity that allows slide
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float MaxSlideVelocityAngle = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	bool AllowSlideUp = false;

	virtual bool IsMovingOnGround() const override;

	void CalcSlideVelocity(float DeltaTime);

	virtual void ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	UFUNCTION(BlueprintCallable, Category = "Ground")
	const FDMLCharacterGroundInfo& GetGroundInfo();

protected:
	FDMLCharacterGroundInfo CachedGroundInfo;
};
