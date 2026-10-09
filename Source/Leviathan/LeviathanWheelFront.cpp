// Copyright Epic Games, Inc. All Rights Reserved.

#include "LeviathanWheelFront.h"
#include "UObject/ConstructorHelpers.h"

ULeviathanWheelFront::ULeviathanWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}