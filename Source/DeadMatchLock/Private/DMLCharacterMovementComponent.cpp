// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLCharacterMovementComponent.h"

#include "Libraries/DMLFunctionLibrary.h"

bool UDMLCharacterMovementComponent::CanCrouchInCurrentState() const
{
	if (!CanEverCrouch())
	{
		return false;
	}
 
	return IsMovingOnGround() && UpdatedComponent && !UpdatedComponent->IsSimulatingPhysics();
}

void UDMLCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		PhysSlide(deltaTime, Iterations);
		break;

	default:
		Super::PhysCustom(deltaTime, Iterations);
		break;
	}
}

void UDMLCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	bWantsToSlide = Flags & FSavedMove_Character::FLAG_Custom_0;
	
	if (bWantsToSlide && !bIsSliding)
		StartSlide();
	else if (!bWantsToSlide && bIsSliding)
		EndSlide();
}

void UDMLCharacterMovementComponent::PhysSlide(float DeltaTime, int32 Iterations)
{
	if (!CharacterOwner || !UpdatedComponent || DeltaTime < MIN_TICK_TIME)
		return;
	
	FHitResult Hit;
	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, &Hit);
	FVector FloorNormal = CurrentFloor.HitResult.ImpactNormal;
	DrawDebugLine(GetWorld(), CharacterOwner->GetActorLocation(), CharacterOwner->GetActorLocation() + FVector::VectorPlaneProject(-GetGravityZ() * GetGravityDirection(), FloorNormal), FColor::Red, false, 0, 0, 5);
	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		CalcSlideVelocity(DeltaTime);
	}
	MoveAlongFloor(Velocity, DeltaTime);

	Iterations++;
	
	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, &Hit);

	FloorSlopeAngle = FMath::RadiansToDegrees(
		FMath::Acos(FVector::DotProduct(FloorNormal, FVector::UpVector))
	);

	const FVector ProjectedVelocity = FVector::VectorPlaneProject(Velocity, FloorNormal);
	DrawDebugLine(GetWorld(), CharacterOwner->GetActorLocation(), CharacterOwner->GetActorLocation() + ProjectedVelocity, FColor::Purple, false, 0, 0, 5);

	const bool SlideUp = FVector::DotProduct(ProjectedVelocity, FVector::UpVector) > 0;

	if (!CurrentFloor.bWalkableFloor || Velocity.Length() < MinSlideStopSpeed || (SlideUp && FloorSlopeAngle > MinSlideSlopeAngle && !AllowSlideUp))
	{
		EndSlide();
		StartNewPhysics(DeltaTime, Iterations);
	}
}
 
FNetworkPredictionData_Client* UDMLCharacterMovementComponent::GetPredictionData_Client() const
{
	if (!ClientPredictionData)
	{
		auto MutableThis = const_cast<UDMLCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Slide(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}
	return ClientPredictionData;
}

void UDMLCharacterMovementComponent::StartSlide()
{
	if (!CanSlide()) return;

	if (FloorSlopeAngle >= MinSlideSlopeAngle)
		Velocity = Velocity.GetSafeNormal() * FMath::Max(MinSlideActivationSpeed, Velocity.Size());

	bCrouchMaintainsBaseLocation = true;
	bIsSliding = true;
	SetMovementMode(MOVE_Custom, CMOVE_Slide);
}

void UDMLCharacterMovementComponent::EndSlide()
{
	if (!bIsSliding) return;
	
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("End Slide")));

	bIsSliding = false;
	SetMovementMode(MOVE_Walking);
}

bool UDMLCharacterMovementComponent::CanSlide()
{
	if (!IsWalking())
	{
		return false;
	}

	const float Speed = Velocity.Size2D();
	const FVector FloorNormal = CurrentFloor.HitResult.ImpactNormal;

	FloorSlopeAngle = FMath::RadiansToDegrees(
		FMath::Acos(FVector::DotProduct(FloorNormal, FVector::UpVector))
	);
	const FVector VelocityProjected = FVector::VectorPlaneProject(Velocity, FloorNormal);
	const bool SlideUp = FVector::DotProduct(VelocityProjected, -GetGravityDirection()) > 0;

	if (Speed >= MinSlideActivationSpeed && !(SlideUp && FloorSlopeAngle > MinSlideSlopeAngle && !AllowSlideUp))
	{
		return true;
	}

	if (!CurrentFloor.IsWalkableFloor())
	{
		return false;
	}


	const FVector SlopeDirection = FVector::VectorPlaneProject(FVector::DownVector, FloorNormal).GetSafeNormal();
	const float SlideVelocityAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(SlopeDirection, VelocityProjected.GetSafeNormal())));
	if (SlopeDirection.IsNearlyZero() || Velocity.IsNearlyZero())
		return false;

	// DrawDebugLine(GetWorld(), CharacterOwner->GetActorLocation(), CharacterOwner->GetActorLocation() + VelocityProjected.GetSafeNormal() * 200, FColor::Red, false, 10, 0, 5);
	// DrawDebugLine(GetWorld(), CharacterOwner->GetActorLocation(), CharacterOwner->GetActorLocation() + SlopeDirection.GetSafeNormal() * 200, FColor::Green, false, 10, 0, 5);

	// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("FloorSlopeAngle: %f\nSlideVelocityAngle: %f"), FloorSlopeAngle, SlideVelocityAngle));
	return FloorSlopeAngle >= MinSlideSlopeAngle && SlideVelocityAngle <= MaxSlideVelocityAngle;
}

bool UDMLCharacterMovementComponent::GetIsSliding() const
{
	return bIsSliding;
}

bool UDMLCharacterMovementComponent::IsMovingOnGround() const
{
	return (MovementMode == MOVE_Walking || MovementMode == MOVE_NavWalking || (MovementMode == MOVE_Custom && CustomMovementMode == CMOVE_Slide)) && UpdatedComponent;
}

void UDMLCharacterMovementComponent::CalcSlideVelocity(float DeltaTime)
{
	if (!HasValidData() || HasAnimRootMotion() || DeltaTime < MIN_TICK_TIME || (CharacterOwner && CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy && !bWasSimulatingRootMotion))
	{
		return;
	}

	RestorePreAdditiveRootMotionVelocity();
	
	float Speed = Velocity.Size2D();
	const FVector FloorNormal = CurrentFloor.HitResult.ImpactNormal;
	const FVector GravityAccel = FVector::VectorPlaneProject(-GetGravityZ() * GetGravityDirection(), FloorNormal);
	const FVector ProjectedVelocity = FVector::VectorPlaneProject(Velocity, FloorNormal);
	const bool SlideUp = FVector::DotProduct(ProjectedVelocity, FVector::UpVector) > 0;
	
	Speed += SlideUp ? -GravityAccel.Size() * DeltaTime : GravityAccel.Size() * DeltaTime;
	Speed -= SlideDeceleration * DeltaTime;

	const FVector InputDirection = Acceleration.GetSafeNormal2D();
	FVector VelocityDirection = Velocity.GetSafeNormal2D();

	if (!InputDirection.IsNearlyZero())
	{
		// Full turn in 1 second at SlideControlFactor = 1
		VelocityDirection = UDMLFunctionLibrary::RotateTowards(VelocityDirection, InputDirection, 180.0f * DeltaTime * SlideControlFactor);
	}
	
	Velocity = VelocityDirection * FMath::Clamp(Speed, 0, GetMaxSpeed());
}

void UDMLCharacterMovementComponent::ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations)
{
	Super::ProcessLanded(Hit, remainingTime, Iterations);
	if (CharacterOwner)
	{
		if (bWantsToSlide)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("Speed = %f"), Velocity.Size2D()));
			StartSlide();
		}
	}
}

void UDMLCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (MovementMode == MOVE_Custom && CustomMovementMode == CMOVE_Slide)
	{
		bCrouchMaintainsBaseLocation = true;
	}
}
