// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TopDown.h"
#include "TopDownGameMode.h"
#include "TopDownPlayerController.h"
#include "TopDownCharacter.h"
#include "CustomHUD.h"
#include "MyActor_Spawner.h"
#include "MyGameStateBase.h"

ATopDownGameMode::ATopDownGameMode()
{
	HUDClass = ACustomHUD::StaticClass();
	GameStateClass = AMyGameStateBase::StaticClass();
	// use our custom PlayerController class
	PlayerControllerClass = ATopDownPlayerController::StaticClass();
	
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	UE_LOG(LogTemp, Warning, TEXT("ATopDownGameMode::ATopDownGameMode"));
	_spawner = nullptr;
	if (GWorld) {	// if not , will crash
		FVector newLocation = FVector(-180.000000, -350.000000, 171.000000);
		TArray<AActor*> list;
		if (GetWorld()) {
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyActor_Spawner::StaticClass(), list);
			if (list.Num() == 0)
				_spawner = GWorld->SpawnActor<AMyActor_Spawner>(AMyActor_Spawner::StaticClass(), newLocation, FRotator::ZeroRotator);

		}
		
	}
}

void ATopDownGameMode::InitGameState()
{
	Super::InitGameState();

}

void ATopDownGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogTemp, Warning, TEXT("ATopDownGameMode::EndPlay"));
	//GWorld->RemoveActor(_spawner, true);
	//_spawner->Destroy();
	if(_spawner)
		GetWorld()->DestroyActor(_spawner, true);
}

