// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TopDown.h"
#include "SStrategyMiniMapWidget.h"
#include "CustomHUD.h"
#include "TopDownPlayerController.h"
#include "MyGameStateBase.h"
#include "Engine/Engine.h"

void SStrategyMiniMapWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill);
	OwnerHUD = InArgs._OwnerHUD;
	bIsMouseButtonDown = false;
}

FReply SStrategyMiniMapWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if( OwnerHUD.IsValid() == false )
	{		
		return FReply::Unhandled();
	}
	ATopDownPlayerController* const StrategyPlayerController = Cast<ATopDownPlayerController>(GEngine->GetFirstLocalPlayerController(OwnerHUD.Get()->GetWorld()));
	if( StrategyPlayerController == nullptr )
	{
		return FReply::Unhandled();
	}
	AMyGameStateBase const* const MyGameState = StrategyPlayerController->GetWorld()->GetGameState<AMyGameStateBase>();
	ACustomHUD* HUD = Cast<ACustomHUD>(StrategyPlayerController->MyHUD);
	if( ( MyGameState == nullptr) || ( HUD == nullptr ) )
	{
		return FReply::Unhandled();
	}

	const float UIScale = HUD->UIScale;
	const float HalfMiniMapWidth = ((MyGameState->MiniMapCamera->MiniMapWidth - HUD->MiniMapMargin) * UIScale) / 2.0f;
	const float HalfMiniMapHeight = ((MyGameState->MiniMapCamera->MiniMapHeight - HUD->MiniMapMargin) * UIScale) / 2.0f;
	 
	FVector2D LocalCoords(MouseEvent.GetScreenSpacePosition() - MyGeometry.AbsolutePosition);
	FVector2D NormalizedMinimapCoords(((LocalCoords.X - HalfMiniMapWidth) / HalfMiniMapWidth), ((LocalCoords.Y - HalfMiniMapHeight) / HalfMiniMapHeight));

	// Use a fixed yaw of 270.0f here instead of calculating (270.0f + MyGameState->MiniMapCamera->GetRootComponent()->GetComponentRotation().Roll).
	const FRotationMatrix RotationMatrix(FRotator(0.0f, 270.0f, 0.0f));
	const FVector OrgCenter = MyGameState->WorldBounds.GetCenter();
	const FVector OrgExt = MyGameState->WorldBounds.GetExtent();
	const FVector Extent = RotationMatrix.TransformPosition(FVector(NormalizedMinimapCoords,0)) * OrgExt;

	FVector CameraTarget = FVector(OrgCenter.X - Extent.X,	OrgCenter.Y - Extent.Y,	OrgCenter.Z + Extent.Z);
	
	StrategyPlayerController->MousePressedOverMinimap(); 
	StrategyPlayerController->SetCameraTarget(CameraTarget);

	bIsMouseButtonDown = true;
	return FReply::Handled();
}

void SStrategyMiniMapWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	bIsMouseButtonDown = false;
	ATopDownPlayerController* const PlayerController = Cast<ATopDownPlayerController>(GEngine->GetFirstLocalPlayerController(OwnerHUD.Get()->GetWorld()));
	if (PlayerController != NULL)
	{
		//PlayerController->MouseLeftMinimap();
	}
}

FReply SStrategyMiniMapWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();
	// Only handle UP if we handled DOWN
	if (bIsMouseButtonDown == true )
	{
		bIsMouseButtonDown = false;
		ATopDownPlayerController* const PlayerController = Cast<ATopDownPlayerController>(GEngine->GetFirstLocalPlayerController(OwnerHUD.Get()->GetWorld()));
		if (PlayerController != NULL)
		{
			//PlayerController->MouseReleasedOverMinimap();
		}
		Reply = FReply::Handled();
	}
	return Reply;
}

FReply SStrategyMiniMapWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) 
{
	if (bIsMouseButtonDown)
	{
		OnMouseButtonDown(MyGeometry,MouseEvent);
	}
	return FReply::Handled();
}

int32 SStrategyMiniMapWidget::OnPaint( const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const
{
	SCompoundWidget::OnPaint( Args, AllottedGeometry, MyClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled );
	if( OwnerHUD.IsValid() == true )
	{
		ATopDownPlayerController* const PC = Cast<ATopDownPlayerController>(GEngine->GetFirstLocalPlayerController(OwnerHUD.Get()->GetWorld()));
		AMyGameStateBase const* const MyGameState = PC && PC->GetWorld() ? PC->GetWorld()->GetGameState<AMyGameStateBase>() : NULL;
		ACustomHUD* const HUD = PC ? Cast<ACustomHUD>(PC->MyHUD) : NULL;
		if (MyGameState && MyGameState->MiniMapCamera.IsValid() && HUD)
		{
			TArray<FVector2D> LinePoints;
			const float HalfMiniMapWidth = (MyGameState->MiniMapCamera->MiniMapWidth - HUD->MiniMapMargin) / 2.0f;
			const float HalfMiniMapHeight = (MyGameState->MiniMapCamera->MiniMapHeight - HUD->MiniMapMargin) / 2.0f;
			const FVector2D MiniMapCenter(HalfMiniMapWidth,HalfMiniMapHeight);

			for (int32 i=0; i < 5; i++)
			{
				LinePoints.Add( MiniMapCenter + HUD->MiniMapPoints[i % 4] * MiniMapCenter );
			}

			FSlateDrawElement::MakeLines( 
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(),
				LinePoints,
				MyClippingRect,
				bParentEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect,
				FColor::White,
				false);
		}
	}
	return LayerId;
}
