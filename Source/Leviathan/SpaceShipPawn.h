// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceShipPawn.generated.h"

class UCameraComponent;
class UStaticMeshComponent;

/** Minimal six-axis ship placeholder for the first spaceflight prototype. */
UCLASS()
class ALeviathanSpaceShipPawn : public APawn
{
	GENERATED_BODY()

public:
	ALeviathanSpaceShipPawn();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, Category="Ship")
	TObjectPtr<UStaticMeshComponent> ShipMesh;

	UPROPERTY(VisibleAnywhere, Category="Ship")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, Category="Flight", meta=(ClampMin="0.0"))
	float FlightAcceleration = 600.0f;

	UPROPERTY(EditAnywhere, Category="Orbit", meta=(ClampMin="1.0"))
	float OrbitRadius = 12000.0f;

	UPROPERTY(EditAnywhere, Category="Orbit", meta=(ClampMin="0.0"))
	float GravitationalParameter = 17280000000.0f;

	float ForwardInput = 0.0f;
	float StrafeInput = 0.0f;
	float VerticalInput = 0.0f;
	float CameraYawInput = 0.0f;
	float CameraPitchInput = 0.0f;
	float CameraYaw = 0.0f;
	float CameraPitch = -12.0f;
	FVector ShipVelocity = FVector::ZeroVector;
	virtual void BeginPlay() override;

	void MoveForward(float Value) { ForwardInput = Value; }
	void MoveRight(float Value) { StrafeInput = Value; }
	void MoveUp(float Value) { VerticalInput = Value; }
	void RotateCameraYaw(float Value) { CameraYawInput = Value; }
	void RotateCameraPitch(float Value) { CameraPitchInput = Value; }
};
