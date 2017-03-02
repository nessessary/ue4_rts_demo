// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "MyActor_Spawner.h"
#include "TopDownGameMode.generated.h"

UCLASS(minimalapi)
class ATopDownGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATopDownGameMode();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	AMyActor_Spawner* _spawner;
};



