// Copyright Epic Games, Inc. All Rights Reserved.

#include "LeviathanWheelRear.h"
#include "UObject/ConstructorHelpers.h"

ULeviathanWheelRear::ULeviathanWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}