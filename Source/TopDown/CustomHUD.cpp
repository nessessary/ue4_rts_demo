// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDown.h"
#include "engine.h"
#include "CustomHUD.h"
#include <algorithm>

void ACustomHUD::DrawHUD()
{
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
}
