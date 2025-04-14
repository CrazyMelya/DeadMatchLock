// Fill out your copyright notice in the Description page of Project Settings.


#include "DMLCharacterMovementComponent.h"

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

void UDMLCharacterMovementComponent::PhysSlide(float DeltaTime, int32 Iterations)
{
	if (!CharacterOwner || !UpdatedComponent)
		return;

	ClearAccumulatedForces();

	// Обработка input (влево/вправо)
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("SlideInput %f, %f"), SlideInput.X, SlideInput.Y));
	if (!SlideInput.IsNearlyZero())
	{
		SlideDirection = (SlideDirection + SlideInput * SlideControlFactor).GetSafeNormal();
	}

	// Направление поверхности
	const FVector FloorNormal = CurrentFloor.HitResult.ImpactNormal;
	FVector SlopeDirection = FVector::VectorPlaneProject(SlideDirection, FloorNormal).GetSafeNormal();

	// Поддержка гравитации по наклону
	const float SlopeAccel = FVector::DotProduct(SlopeDirection, FVector::DownVector); // > 0 значит склон вниз
	if (SlopeAccel > KINDA_SMALL_NUMBER)
	{
		float GravityAssist = FMath::Abs(GetGravityZ()) * SlopeAccel * DeltaTime;
		SlideSpeed += GravityAssist;
	}

	// Замедление
	SlideSpeed = FMath::Max(SlideSpeed - SlideDeceleration * DeltaTime, 0.f);

	// Финальное смещение
	const FVector MoveDelta = SlopeDirection * SlideSpeed * DeltaTime;

	// Слайдим
	FHitResult Hit;
	SafeMoveUpdatedComponent(MoveDelta, UpdatedComponent->GetComponentQuat(), true, Hit);

	// Завершение слайда при коллизии или малой скорости
	if (SlideSpeed < MinSlideStopSpeed || Hit.IsValidBlockingHit())
	{
		EndSlide();
		return;
	}

	// Проверка отрыва от земли
	if (!IsFalling() && !IsWalkable(Hit))
	{
		// Пробуем обновить пол, если вдруг потеряли контакт
		FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, false);

		if (!CurrentFloor.IsWalkableFloor())
		{
			EndSlide();
		}
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
	if (!CharacterOwner || MovementMode != MOVE_Walking)
		return;

	const FVector HorizontalVelocity = Velocity.GetSafeNormal2D();
	const float Speed = Velocity.Size2D();

	// Получаем текущую поверхность под ногами
	const FFindFloorResult& Floor = CurrentFloor;
	if (!Floor.IsWalkableFloor())
		return;

	const FVector FloorNormal = Floor.HitResult.ImpactNormal;

	// Угол между вверх и нормалью
	const float FloorSlopeAngle = FMath::RadiansToDegrees(
		FMath::Acos(FVector::DotProduct(FloorNormal, FVector::UpVector))
	);

	const bool bSlideFromSpeed = Speed >= MinSlideActivationSpeed;
	const bool bSlideFromSlope = FloorSlopeAngle >= MinSlideSlopeAngle;

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Floor Slope Angle = %f"), FloorSlopeAngle));
	
	if (!bSlideFromSpeed && !bSlideFromSlope)
		return;

	FVector FloorSlopeDirection = FVector::CrossProduct(FloorNormal, FVector::CrossProduct(FloorNormal, FVector::UpVector)).GetSafeNormal();

	// Направление слайда: если движемся — учитывать его, иначе просто по уклону
	if (bSlideFromSpeed && FVector::DotProduct(FloorSlopeDirection, HorizontalVelocity) < 0.5f)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("SlideInput %f, %f"), SlideInput.X, SlideInput.Y));

		SlideDirection = (HorizontalVelocity + FloorSlopeDirection * 0.5f).GetSafeNormal();
	}
	else
	{
		SlideDirection = FloorSlopeDirection;
	}

	SlideSpeed = Speed > 1.f ? Speed : 0.f; // если стоим — стартуем с нулевой скорости

	bIsSliding = true;
	SetMovementMode(MOVE_Custom, CMOVE_Slide);
}

void UDMLCharacterMovementComponent::EndSlide()
{
	bIsSliding = false;
	SetMovementMode(MOVE_Walking);
}

bool UDMLCharacterMovementComponent::CanSlide()
{
	if (IsFalling())
	{
		return false;
	}

	const float Speed = Velocity.Size2D();

	if (Speed >= MinSlideActivationSpeed)
	{
		return true;
	}

	if (!CurrentFloor.IsWalkableFloor())
	{
		return false;
	}

	const FVector FloorNormal = CurrentFloor.HitResult.ImpactNormal;
	const float FloorSlopeAngle = FMath::RadiansToDegrees(
		FMath::Acos(FVector::DotProduct(FloorNormal, FVector::UpVector))
	);

	return FloorSlopeAngle >= MinSlideSlopeAngle;
}
