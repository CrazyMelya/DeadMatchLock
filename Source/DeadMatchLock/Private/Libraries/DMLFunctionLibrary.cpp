// Fill out your copyright notice in the Description page of Project Settings.


#include "Libraries/DMLFunctionLibrary.h"


FVector UDMLFunctionLibrary::RotateTowards(FVector From, FVector To, float MaxAngleDegrees)
{
	From = From.GetSafeNormal();
	To = To.GetSafeNormal();

	float AngleBetween = FMath::Acos(FVector::DotProduct(From, To));
	float AngleDegrees = FMath::RadiansToDegrees(AngleBetween);
	
	if (AngleDegrees < KINDA_SMALL_NUMBER)
		return To;
	
	float RotationAngle = FMath::Min(MaxAngleDegrees, AngleDegrees);
	
	FVector RotationAxis = FVector::CrossProduct(From, To).GetSafeNormal();
	if (!RotationAxis.IsNearlyZero())
	{
		return From.RotateAngleAxis(RotationAngle, RotationAxis);
	}
	else
	{
		FVector ArbitraryAxis = From.GetSafeNormal();
		FVector Up = FVector::UpVector;
	
		if (FMath::Abs(FVector::DotProduct(Up, ArbitraryAxis)) > 0.99f)
		{
			Up = FVector::RightVector;
		}
		RotationAxis = FVector::CrossProduct(ArbitraryAxis, Up).GetSafeNormal();
		return From.RotateAngleAxis(RotationAngle, RotationAxis);
	}
}
