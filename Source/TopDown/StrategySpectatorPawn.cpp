// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.


#include "TopDown.h"
#include "StrategySpectatorPawn.h"
#include "StrategyCameraComponent.h"
#include "StrategySpectatorPawnMovement.h"

AStrategySpectatorPawn::AStrategySpectatorPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UStrategySpectatorPawnMovement>(Super::MovementComponentName))
{
	GetCollisionComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	bAddDefaultMovementBindings = false;
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->SetupAttachment(GetCollisionComponent());
	//OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 400.f;
	OurCameraSpringArm->bAbsoluteRotation = true;
	OurCameraSpringArm->CameraLagSpeed = 3.0f;
	OurCameraSpringArm->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	OurCameraSpringArm->TargetArmLength = 800.f;
	OurCameraSpringArm->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	OurCameraSpringArm->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	StrategyCameraComponent = CreateDefaultSubobject<UStrategyCameraComponent>(TEXT("StrategyCameraComponent"));
	StrategyCameraComponent->SetupAttachment(OurCameraSpringArm);
}

void AStrategySpectatorPawn::OnMouseScrollUp()
{
	StrategyCameraComponent->OnZoomIn();
}

void AStrategySpectatorPawn::OnMouseScrollDown()
{
	StrategyCameraComponent->OnZoomOut();
}


void AStrategySpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &AStrategySpectatorPawn::OnMouseScrollUp);
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &AStrategySpectatorPawn::OnMouseScrollDown);

	PlayerInputComponent->BindAxis("MoveForward", this, &AStrategySpectatorPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStrategySpectatorPawn::MoveRight);
}


void AStrategySpectatorPawn::MoveForward(float Val)
{
	StrategyCameraComponent->MoveForward( Val );
}


void AStrategySpectatorPawn::MoveRight(float Val)
{
	StrategyCameraComponent->MoveRight( Val );
}

UStrategyCameraComponent* AStrategySpectatorPawn::GetStrategyCameraComponent()
{
	check( StrategyCameraComponent != NULL );
	return StrategyCameraComponent;
}

