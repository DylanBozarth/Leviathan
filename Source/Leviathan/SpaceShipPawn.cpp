// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceShipPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

ALeviathanSpaceShipPawn::ALeviathanSpaceShipPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMesh;
	ShipMesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMesh(TEXT("/Engine/BasicShapes/Cone.Cone"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> BasicMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (CubeMesh.Succeeded())
	{
		ShipMesh->SetStaticMesh(CubeMesh.Object);
		ShipMesh->SetRelativeScale3D(FVector(2.2f, 0.62f, 0.42f));
	}

	auto SetShipColor = [](UStaticMeshComponent* Mesh, const FLinearColor& Color)
	{
		if (BasicMaterial.Succeeded())
		{
			if (UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(BasicMaterial.Object, Mesh))
			{
				Material->SetVectorParameterValue(TEXT("Color"), Color);
				Mesh->SetMaterial(0, Material);
			}
		}
	};
	SetShipColor(ShipMesh, FLinearColor(0.025f, 0.16f, 0.24f));

	if (CubeMesh.Succeeded())
	{
		UStaticMeshComponent* LeftWing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWing"));
		LeftWing->SetupAttachment(RootComponent);
		LeftWing->SetStaticMesh(CubeMesh.Object);
		LeftWing->SetRelativeLocation(FVector(-20.0f, -105.0f, 0.0f));
		LeftWing->SetRelativeScale3D(FVector(0.9f, 0.9f, 0.12f));
		LeftWing->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetShipColor(LeftWing, FLinearColor(0.035f, 0.42f, 0.58f));

		UStaticMeshComponent* RightWing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWing"));
		RightWing->SetupAttachment(RootComponent);
		RightWing->SetStaticMesh(CubeMesh.Object);
		RightWing->SetRelativeLocation(FVector(-20.0f, 105.0f, 0.0f));
		RightWing->SetRelativeScale3D(FVector(0.9f, 0.9f, 0.12f));
		RightWing->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetShipColor(RightWing, FLinearColor(0.035f, 0.42f, 0.58f));

		for (int32 Side = -1; Side <= 1; Side += 2)
		{
			UStaticMeshComponent* Engine = CreateDefaultSubobject<UStaticMeshComponent>(Side < 0 ? TEXT("LeftEngine") : TEXT("RightEngine"));
			Engine->SetupAttachment(RootComponent);
			Engine->SetStaticMesh(CubeMesh.Object);
			Engine->SetRelativeLocation(FVector(-112.0f, Side * 36.0f, 0.0f));
			Engine->SetRelativeScale3D(FVector(0.22f, 0.16f, 0.16f));
			Engine->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SetShipColor(Engine, FLinearColor(1.0f, 0.24f, 0.035f));
		}
	}

	if (SphereMesh.Succeeded())
	{
		UStaticMeshComponent* Cockpit = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cockpit"));
		Cockpit->SetupAttachment(RootComponent);
		Cockpit->SetStaticMesh(SphereMesh.Object);
		Cockpit->SetRelativeLocation(FVector(28.0f, 0.0f, 36.0f));
		Cockpit->SetRelativeScale3D(FVector(0.48f, 0.32f, 0.28f));
		Cockpit->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetShipColor(Cockpit, FLinearColor(0.1f, 0.8f, 1.0f));
	}

	if (ConeMesh.Succeeded())
	{
		UStaticMeshComponent* Nose = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Nose"));
		Nose->SetupAttachment(RootComponent);
		Nose->SetStaticMesh(ConeMesh.Object);
		Nose->SetRelativeLocation(FVector(115.0f, 0.0f, 0.0f));
		Nose->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		Nose->SetRelativeScale3D(FVector(0.2f, 0.25f, 0.45f));
		Nose->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetShipColor(Nose, FLinearColor(0.12f, 0.62f, 0.76f));
	}

	USpringArmComponent* CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 700.0f;
	CameraBoom->SetRelativeLocation(FVector(-100.0f, 0.0f, 180.0f));
	CameraBoom->SetRelativeRotation(FRotator(CameraPitch, CameraYaw, 0.0f));
	CameraBoom->bDoCollisionTest = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

void ALeviathanSpaceShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("ShipForward"), this, &ALeviathanSpaceShipPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("ShipStrafe"), this, &ALeviathanSpaceShipPawn::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("ShipVertical"), this, &ALeviathanSpaceShipPawn::MoveUp);
	PlayerInputComponent->BindAxis(TEXT("ShipCameraYaw"), this, &ALeviathanSpaceShipPawn::RotateCameraYaw);
	PlayerInputComponent->BindAxis(TEXT("ShipCameraPitch"), this, &ALeviathanSpaceShipPawn::RotateCameraPitch);
}

void ALeviathanSpaceShipPawn::BeginPlay()
{
	Super::BeginPlay();

	// Start one circular orbit from the sun at the origin.
	SetActorLocation(FVector(OrbitRadius, 0.0f, 0.0f));
	ShipVelocity = FVector(0.0f, FMath::Sqrt(GravitationalParameter / OrbitRadius), 0.0f);
	CameraYaw = 90.0f;
}

void ALeviathanSpaceShipPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CameraYaw += CameraYawInput;
	CameraPitch = FMath::Clamp(CameraPitch - CameraPitchInput, -80.0f, 35.0f);
	if (USpringArmComponent* CameraBoom = FindComponentByClass<USpringArmComponent>())
	{
		CameraBoom->SetRelativeRotation(FRotator(CameraPitch, CameraYaw, 0.0f));
	}

	const FVector LocalInput(ForwardInput, StrafeInput, VerticalInput);
	if (!LocalInput.IsNearlyZero())
	{
		ShipVelocity += GetActorTransform().TransformVectorNoScale(LocalInput.GetClampedToMaxSize(1.0f)) * FlightAcceleration * DeltaSeconds;
	}

	const FVector SunDirection = -GetActorLocation();
	const float DistanceSquared = FMath::Max(SunDirection.SizeSquared(), 10000.0f);
	const FVector GravityAcceleration = SunDirection.GetSafeNormal() * (GravitationalParameter / DistanceSquared);
	ShipVelocity += GravityAcceleration * DeltaSeconds;
	AddActorWorldOffset(ShipVelocity * DeltaSeconds, false);
	if (!ShipVelocity.IsNearlyZero())
	{
		SetActorRotation(ShipVelocity.ToOrientationRotator());
	}
}
