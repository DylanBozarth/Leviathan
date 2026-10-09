// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceGameMode.h"
#include "SpaceShipPawn.h"
#include "Components/PointLightComponent.h"
#include "Engine/PointLight.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h"

void ALeviathanSpaceGameMode::StartPlay()
{
	Super::StartPlay();

	if (UWorld* World = GetWorld())
	{
		const FVector SunLocation = FVector::ZeroVector;
		const FLinearColor SunColor(1.0f, 0.68f, 0.28f);
		UMaterialInterface* BasicShapeMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
		if (AStaticMeshActor* SunVisual = World->SpawnActor<AStaticMeshActor>(SunLocation, FRotator::ZeroRotator))
		{
			if (UStaticMesh* SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere")))
			{
				SunVisual->GetStaticMeshComponent()->SetStaticMesh(SphereMesh);
				SunVisual->SetActorScale3D(FVector(22.0f));
				SunVisual->GetStaticMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				if (BasicShapeMaterial)
				{
					SunVisual->GetStaticMeshComponent()->SetMaterial(0, BasicShapeMaterial);
				}
				if (UMaterialInstanceDynamic* SunMaterial = SunVisual->GetStaticMeshComponent()->CreateAndSetMaterialInstanceDynamic(0))
				{
					SunMaterial->SetVectorParameterValue(TEXT("Color"), SunColor);
					SunMaterial->SetVectorParameterValue(TEXT("EmissiveColor"), SunColor * 8.0f);
				}
			}
		}

		// Offset the source slightly so it lights the visible surface of the sun sphere.
		if (APointLight* DistantLight = World->SpawnActor<APointLight>(SunLocation + FVector(1200.0f, 0.0f, 0.0f), FRotator::ZeroRotator))
		{
			if (UPointLightComponent* LightComponent = Cast<UPointLightComponent>(DistantLight->GetLightComponent()))
			{
				LightComponent->SetIntensity(5000000.0f);
				LightComponent->SetAttenuationRadius(100000.0f);
				LightComponent->SetLightColor(SunColor);
				LightComponent->SetSourceRadius(1400.0f);
			}
		}
	}
}

ALeviathanSpaceGameMode::ALeviathanSpaceGameMode()
{
	DefaultPawnClass = ALeviathanSpaceShipPawn::StaticClass();
}
