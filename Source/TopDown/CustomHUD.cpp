// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDown.h"
#include "engine.h"
#include "CustomHUD.h"
#include <algorithm>
#include "SlateBasics.h"
#include "SButton.h"
#include "STextBlock.h"
#include "SlateExtras.h"
#include "TopDownPlayerController.h"
#include "MyGameStateBase.h"
#include "StrategyHelpers.h"

void ACustomHUD::DrawHUD()
{
	if (GEngine && GEngine->GameViewport)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		UIScale = ViewportSize.X / 2048.0f;
	}

	Super::DrawHUD();

	//Canvas->DrawText(GEngine->GetSmallFont(), TEXT("Test string to be printed to screen"), 10, 10);
	FCanvasBoxItem ProgressBar(FVector2D(5, 25), FVector2D(50, 5));
	Canvas->DrawItem(ProgressBar);
	DrawRect(FLinearColor::Blue, 5, 25, 30, 5);
	
	if (_left.X !=0 && _right.X !=0) 
	{
		//_left = Canvas->Project(_left);
		//_left.X -= 482;
		//_right = Canvas->Project(_right);
		//_right.X -= 482;
		DrawRect(FLinearColor::Green, std::min(_left.X, _right.X), std::min(_left.Y, _right.Y),
			fabs(_right.X-_left.X), fabs(_right.Y-_left.Y));
		FString string = FString::Printf(TEXT("%.2f,%.2f %.2f,%.2f"), _left.X, _left.Y,_right.X, _right.Y);
		Canvas->DrawText(GEngine->GetSmallFont(), FText::FromString(string), 10, 10);
	}
	//DrawRect(FLinearColor::Green, 50,50, 60,60);

	BuildWidget();
	DrawMiniMap();
}

void ACustomHUD::BuildWidget()
{
	if (MiniMapWidget.IsValid()) {
		return;
	}
	TSharedRef<SVerticalBox> widget = SNew(SVerticalBox)
	+ SVerticalBox::Slot()

	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	[

		SNew(SOverlay)
		+ SOverlay::Slot()
		.VAlign(VAlign_Bottom)
		.HAlign(HAlign_Left)
		.Padding(FMargin(40, 0, 0, 40))
		[
			SNew(SBorder)
			//.BorderImage(&HUDStyle->MinimapFrameBrush)
			.Padding(FMargin(0))
			[
				SNew(SBox)
				.Padding(FMargin(10, 10))
				.WidthOverride_Lambda([]() {return FOptionalSize(200.f); })
				.HeightOverride_Lambda([]() {return FOptionalSize(200.f); })
				[
					SAssignNew(MiniMapWidget, SStrategyMiniMapWidget)
					.OwnerHUD(this)
				]
			]
		]
	];
	//GEngine->GameViewport->AddViewportWidgetForPlayer(GetLocalPlayer(), widget, 1);
	GEngine->GameViewport->AddViewportWidgetContent(widget);
}

void ACustomHUD::DrawMiniMap()
{
	MiniMapMargin = 40;
	const ATopDownPlayerController* const PC = Cast<ATopDownPlayerController>(PlayerOwner);
	AMyGameStateBase const* const MyGameState = GetWorld()->GetGameState<AMyGameStateBase>();

	if (PC && MyGameState && MyGameState->MiniMapCamera.IsValid())
	{
		UTexture* MiniMapTexture = Cast<UTexture>(MyGameState->MiniMapCamera->GetCaptureComponent2D()->TextureTarget);
		const float MapWidth = (MyGameState->MiniMapCamera->MiniMapWidth - MiniMapMargin) * UIScale;
		const float MapHeight = (MyGameState->MiniMapCamera->MiniMapHeight - MiniMapMargin) * UIScale;
		const FVector WorldCenter = MyGameState->WorldBounds.GetCenter();
		const FVector WorldExtent = MyGameState->WorldBounds.GetExtent();
		// Use a fixed yaw of 270.0f here instead of calculating (270.0f + MyGameState->MiniMapCamera->GetRootComponent()->GetComponentRotation().Roll).
		const FRotationMatrix RotationMatrix(FRotator(0.0f, 270.0f, 0.0f));
		const FVector2D Offset(MiniMapMargin * UIScale + (MapWidth / 2.0f), Canvas->ClipY - (MapHeight / 2.0f) - MiniMapMargin * UIScale);

		if (MiniMapTexture)
		{
			FCanvasTileItem MapTileItem(FVector2D(0.0f, 0.0f), FVector2D(0.0f, 0.0f), FLinearColor::White);
			MapTileItem.Texture = MiniMapTexture->Resource;
			MapTileItem.Size = FVector2D(MapWidth, MapHeight);
			MapTileItem.BlendMode = SE_BLEND_Opaque;
			Canvas->DrawItem(MapTileItem, FVector2D(MiniMapMargin * UIScale, Canvas->ClipY - MapHeight - MiniMapMargin * UIScale));
		}
		//FCanvasTileItem TileItem(FVector2D(0.0f, 0.0f), FVector2D(0.0f, 0.0f), FLinearColor::White);
		//TileItem.Size = FVector2D(6 * UIScale, 6 * UIScale);
		//for (FConstPawnIterator Iterator = GetWorld()->GetPawnIterator(); Iterator; ++Iterator)
		//{
		//	AStrategyChar* TestChar = Cast<AStrategyChar>(*Iterator);
		//	if (TestChar != NULL && TestChar->GetHealth() > 0)
		//	{
		//		AStrategyAIController* AIController = Cast<AStrategyAIController>(TestChar->Controller);
		//		if (AIController != NULL && AIController->IsLogicEnabled())
		//		{
		//			FLinearColor DrawColor;
		//			if (PC != NULL && TestChar->GetTeamNum() == PC->GetTeamNum())
		//			{
		//				DrawColor = FColor(49, 137, 253, 255);
		//			}
		//			else
		//			{
		//				DrawColor = FColor(242, 114, 16, 255);
		//			}
		//			const FVector CenterRelativeLocation = RotationMatrix.TransformPosition(TestChar->GetActorLocation() - WorldCenter);
		//			const FVector2D MiniMapPoint = FVector2D(CenterRelativeLocation.X / WorldExtent.X, CenterRelativeLocation.Y / WorldExtent.Y);
		//			TileItem.SetColor(DrawColor);
		//			Canvas->DrawItem(TileItem, FVector2D(Offset.X + MiniMapPoint.X * (MapWidth / 2.0f), Offset.Y + MiniMapPoint.Y * (MapHeight / 2.0f)));
		//		}
		//	}
		//}


		ULocalPlayer* MyPlayer = Cast<ULocalPlayer>(PC->Player);
		FVector2D ScreenCorners[4] = { FVector2D(0, 0), FVector2D(Canvas->ClipX, 0), FVector2D(Canvas->ClipX, Canvas->ClipY), FVector2D(0, Canvas->ClipY) };
		const FPlane GroundPlane = FPlane(FVector(0, 0, MyGameState->WorldBounds.Max.Z), FVector::UpVector);
		for (int32 i = 0; i < 4; i++)
		{
			FVector RayOrigin, RayDirection;
			FStrategyHelpers::DeprojectScreenToWorld(ScreenCorners[i], MyPlayer, RayOrigin, RayDirection);
			const FVector GroundPoint = FStrategyHelpers::IntersectRayWithPlane(RayOrigin, RayDirection, GroundPlane);
			const FVector CenterRelativeLocation = RotationMatrix.TransformPosition(GroundPoint - WorldCenter);
			MiniMapPoints[i] = FVector2D(CenterRelativeLocation.X / WorldExtent.X, CenterRelativeLocation.Y / WorldExtent.Y);
		}
	}

}
