// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameStateBase.h"
#include "StrategyMiniMapCapture.h"
#include "MyGameStateBase.generated.h"

enum GameTeam {
	PLAYER,
	ENEMY,
};
/**
 * 
 */
UCLASS()
class TOPDOWN_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_UCLASS_BODY()
	
public:
	/** Mini map camera component. */
	TWeakObjectPtr<AStrategyMiniMapCapture> MiniMapCamera;
	//AStrategyMiniMapCapture* MiniMapCamera;

	/** World bounds for mini map & camera movement. */
	FBox WorldBounds;
	
	
};
