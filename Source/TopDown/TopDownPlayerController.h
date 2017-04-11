// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "StrategySpectatorPawn.h"
#include "StrategyCameraComponent.h"
#include "CustomHUD.h"
#include "TopDownPlayerController.generated.h"

UCLASS()
class ATopDownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATopDownPlayerController();
	virtual void BeginPlay() override;

	void MouseLeftMinimap();
	void MousePressedOverMinimap();
	void MouseReleasedOverMinimap();
	AStrategySpectatorPawn* GetStrategySpectatorPawn() const;
	UStrategyCameraComponent* GetCameraComponent() const;
	void SetCameraTarget(const FVector& CameraTarget);

private:
	FVector2D GetCursor();


protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	bool is_selected;
	bool bPreMouseState;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void ProcessPlayerInput(const float DeltaTime, const bool bGamePaused) override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();


};


