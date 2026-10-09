// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceGameMode.generated.h"

UCLASS()
class ALeviathanSpaceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALeviathanSpaceGameMode();
	virtual void StartPlay() override;
};
