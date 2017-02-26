// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TopDown.h"
#include "TopDownGameMode.h"
#include "TopDownPlayerController.h"
#include "TopDownCharacter.h"
#include "CustomHUD.h"

ATopDownGameMode::ATopDownGameMode()
{
	HUDClass = ACustomHUD::StaticClass();

	// use our custom PlayerController class
	PlayerControllerClass = ATopDownPlayerController::StaticClass();
	
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass2(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
}