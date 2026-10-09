// Copyright Epic Games, Inc. All Rights Reserved.

#include "LeviathanGameMode.h"
#include "LeviathanPlayerController.h"

ALeviathanGameMode::ALeviathanGameMode()
{
	PlayerControllerClass = ALeviathanPlayerController::StaticClass();
}
