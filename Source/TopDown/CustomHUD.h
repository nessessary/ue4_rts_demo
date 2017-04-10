// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "SStrategyMiniMapWidget.h"
#include "CustomHUD.generated.h"

/**
 * 
 */
UCLASS()
class ACustomHUD : public AHUD
{
	GENERATED_BODY()
	
	
	
	
public:
	virtual void DrawHUD() override;
	void BuildWidget();

	TSharedPtr<SStrategyMiniMapWidget> MiniMapWidget;

	FVector2D _left;
	FVector2D _right;
};
