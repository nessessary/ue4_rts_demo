// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.


#include "TopDown.h"
#include "StrategySpectatorPawnMovement.h"
#include "StrategyCameraComponent.h"
#include "StrategySpectatorPawn.h"
#include "TopDownPlayerController.h"

UStrategySpectatorPawnMovement::UStrategySpectatorPawnMovement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bInitialLocationSet(false)
{
	MaxSpeed  = 16000.f;
	Acceleration = 5000.f;
	Deceleration = 4000.f;
}


void UStrategySpectatorPawnMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	ATopDownPlayerController* PlayerController = Cast<ATopDownPlayerController>(PawnOwner->GetController());
	if (PlayerController && PlayerController->IsLocalController())
	{
		if (!bInitialLocationSet)
		{
			PawnOwner->SetActorRotation(PlayerController->GetControlRotation());
			PawnOwner->SetActorLocation(PlayerController->GetSpawnLocation());
			bInitialLocationSet = true;
		}

		FVector MyLocation = UpdatedComponent->GetComponentLocation();
		AStrategySpectatorPawn* SpectatorPawn = Cast<AStrategySpectatorPawn>(PlayerController->GetSpectatorPawn());
		if( ( SpectatorPawn != NULL ) && ( SpectatorPawn->GetStrategyCameraComponent() != NULL) )
		{
			SpectatorPawn->GetStrategyCameraComponent()->ClampCameraLocation(PlayerController, MyLocation);
		}
		UpdatedComponent->SetWorldLocation(MyLocation, false);
	}
}

