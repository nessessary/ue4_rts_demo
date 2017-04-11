// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TopDown.h"
#include "engine.h"
#include "TopDownPlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "TopDownCharacter.h"

#include <algorithm>
#include "MyGameStateBase.h"
#include "StrategyMiniMapCapture.h"

ATopDownPlayerController::ATopDownPlayerController()
{
	is_selected = false;
	bShowMouseCursor = true;
	bPreMouseState = !bShowMouseCursor;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}


void ATopDownPlayerController::BeginPlay()
{
	Super::BeginPlay();
	APawn* pawn = GetPawn();
	this->ChangeState(NAME_Spectating);
	//Possess(pawn);
	//SetSpectatorPawn((ASpectatorPawn*)pawn);
	FVector pos2 = FVector(100, 100, 1000);
	SetCameraTarget(pos2);

	// Init Capture
	AMyGameStateBase* game_state = GetWorld()->GetGameState<AMyGameStateBase>();
	FVector pos = FVector(0, 100, 2000);
	game_state->MiniMapCamera = GetWorld()->SpawnActor<AStrategyMiniMapCapture>(
		AStrategyMiniMapCapture::StaticClass(), pos, FRotator::ZeroRotator);
}

FVector2D ATopDownPlayerController::GetCursor()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	FVector2D pos;
	pos.X = 10;
	pos.Y = 10;
	FVector pos1;
	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		pos1.X = Hit.ImpactPoint.X;
		pos1.Y = Hit.ImpactPoint.Y;
		pos1.Z = Hit.ImpactPoint.Z;
	}
	ProjectWorldLocationToScreen(pos1, pos);
	return pos;
}

bool PtInRect(FVector2D pt, FVector2D left, FVector2D right) {
	bool bRet = false;
	if (pt.X >= std::min(left.X, right.X) && pt.X <= std::max(left.X, right.X)
		&& pt.Y >= std::min(left.Y, right.Y) && pt.Y <= std::max(left.Y, right.Y))
	{
		bRet = true;
	}
	return bRet;
}
void ATopDownPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	ACustomHUD* pHud = Cast<ACustomHUD>(GetHUD());

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		pHud->_right = GetCursor();
		if (bPreMouseState != bMoveToMouseCursor) {
			//UE_LOG(LogTemp, Warning, TEXT("abc"));
			pHud->_left = pHud->_right;
		}

		//FString str = FString::Printf(TEXT("%.2f, %.2f | %.2f, %.2f"), pHud->_left.X, pHud->_left.Y, pHud->_right.X, pHud->_right.Y);
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, str);



		if(is_selected)
			MoveToMouseCursor();
	}
	else
	{
		TArray<AActor*> list;
		if (GetWorld()) {
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATopDownCharacter::StaticClass(), list);
		}
		for (int32 i = 0; i < list.Num(); i++) {
			// 鼠标释放事件
			// 判断角色点在选择矩形内
			ATopDownCharacter* MyPawn = Cast<ATopDownCharacter>(list[i]);
			if (MyPawn) {
				FVector pos = MyPawn->GetActorLocation();
				FVector2D pos1(pos.X, pos.Y);
				ProjectWorldLocationToScreen(pos, pos1);
				//FString str = FString::Printf(TEXT("%.2f, %.2f "), pos1.X, pos1.Y);
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, str);
				if (PtInRect(pos1, pHud->_left, pHud->_right)) {
					is_selected = true;
					// 人物选择状态
					// ...
					MyPawn->Select(is_selected);
				}
			}
		}

		pHud->_left = pHud->_right = FVector2D(0,0);
	}
	bPreMouseState = bMoveToMouseCursor;

}

void ATopDownPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ATopDownPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ATopDownPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATopDownPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ATopDownPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &ATopDownPlayerController::OnResetVR);
}

void ATopDownPlayerController::ProcessPlayerInput(const float DeltaTime, const bool bGamePaused)
{
	Super::ProcessPlayerInput(DeltaTime, bGamePaused);

	if (1)
	{
		const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
		AStrategySpectatorPawn* StrategyPawn = GetStrategySpectatorPawn();
		if ((StrategyPawn != NULL) && (LocalPlayer != NULL))
		{
			// Create the bounds for the minimap so we can add it as a 'no scroll' zone.
			ACustomHUD* const HUD = Cast<ACustomHUD>(GetHUD());
			AMyGameStateBase const* const MyGameState = GetWorld()->GetGameState<AMyGameStateBase>();
			if ((MyGameState != NULL) && (MyGameState->MiniMapCamera.IsValid() == true))
			{
				if (LocalPlayer->ViewportClient != NULL)
				{
					const FIntPoint ViewportSize = LocalPlayer->ViewportClient->Viewport->GetSizeXY();
					const uint32 ViewTop = FMath::TruncToInt(LocalPlayer->Origin.Y * ViewportSize.Y);
					const uint32 ViewBottom = ViewTop + FMath::TruncToInt(LocalPlayer->Size.Y * ViewportSize.Y);

					FVector TopLeft(HUD->MiniMapMargin, ViewBottom - HUD->MiniMapMargin - MyGameState->MiniMapCamera->MiniMapHeight, 0);
					FVector BottomRight((int32)MyGameState->MiniMapCamera->MiniMapWidth, MyGameState->MiniMapCamera->MiniMapHeight, 0);
					FBox MiniMapBounds(TopLeft, TopLeft + BottomRight);
					StrategyPawn->GetStrategyCameraComponent()->AddNoScrollZone(MiniMapBounds);
					StrategyPawn->GetStrategyCameraComponent()->UpdateCameraMovement(this);
				}
			}
		}
	}
}

void ATopDownPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATopDownPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (ATopDownCharacter* MyPawn = Cast<ATopDownCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UNavigationSystem::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ATopDownPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

int g_switch = 0;
void ATopDownPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	TArray<AActor*> list;
	if (GetWorld()) {
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATopDownCharacter::StaticClass(), list);
	}
	for (int32 i = 0; i < list.Num(); i++) 
	//if(list.Num()>1)
	{
		ATopDownCharacter* const MyPawn = (ATopDownCharacter* )list[i];
		if (MyPawn && MyPawn->IsMouseSelected())
		{
			//MyPawn->UnPossessed();
			////if (g_changePawn == 1) 
			//{
			//	// contorller attach pawn
			//	this->SetPawn(MyPawn);
			//	// attach controller
			//	MyPawn->PossessedBy(this);
			//	// Modify MovementComp's MovementMode
			//	((ATopDownCharacter*)MyPawn)->Restart();
			//}
			UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
			float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

			// We need to issue move command only if far enough in order for walk animation to play correctly
			if (NavSys && (Distance > 120.0f))
			{
				if (MyPawn->GetController() != (AController*)this) {
					MyPawn->PlayAI->Possess(MyPawn);
					MyPawn->PossessedBy(MyPawn->PlayAI);
					NavSys->SimpleMoveToLocation(MyPawn->GetController(), DestLocation);
				}
				else {
					NavSys->SimpleMoveToLocation(this, DestLocation);
				}

			}
		}
	}
	//APawn* const MyPawn = GetPawn();
	//if (MyPawn)
	//{
	//	UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
	//	float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

	//	// We need to issue move command only if far enough in order for walk animation to play correctly
	//	if (NavSys && (Distance > 120.0f))
	//	{
	//		NavSys->SimpleMoveToLocation(this, DestLocation);
	//	}
	//}
}

void ATopDownPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;

}

void ATopDownPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void ATopDownPlayerController::MouseLeftMinimap()
{
	if (GetCameraComponent() != NULL)
	{
		GetCameraComponent()->EndSwipeNow();
	}
}

void ATopDownPlayerController::MousePressedOverMinimap()
{
	if (GetCameraComponent() != NULL)
	{
		GetCameraComponent()->EndSwipeNow();
	}
}

void ATopDownPlayerController::MouseReleasedOverMinimap()
{
	if (GetCameraComponent() != NULL)
	{
		GetCameraComponent()->EndSwipeNow();
	}
}

AStrategySpectatorPawn* ATopDownPlayerController::GetStrategySpectatorPawn() const
{
	return Cast<AStrategySpectatorPawn>(GetSpectatorPawn());
}

UStrategyCameraComponent* ATopDownPlayerController::GetCameraComponent() const
{
	UStrategyCameraComponent* CameraComponent = NULL;
	if (GetStrategySpectatorPawn() != NULL)
	{
		CameraComponent = GetStrategySpectatorPawn()->GetStrategyCameraComponent();
	}
	return CameraComponent;

}

void ATopDownPlayerController::SetCameraTarget(const FVector& CameraTarget)
{
	if (GetCameraComponent() != NULL)
	{
		GetCameraComponent()->SetCameraTarget(CameraTarget);
	}
}

