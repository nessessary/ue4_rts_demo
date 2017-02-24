// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDown.h"
#include "MyActor_Spawner.h"


// Sets default values
AMyActor_Spawner::AMyActor_Spawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"), false);

	CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"), false)->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AMyActor_Spawner::BeginPlay()
{
	Super::BeginPlay();

	// 创建一个定时器
	FTimerHandle SpawnTimer;
	//GetWorldTimerManager().SetTimer(SpawnTimer, this, &AMyActor_Spawner::SpawnSingleZombie, 2, true);
}

// Called every frame
void AMyActor_Spawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AMyActor_Spawner::SpawnSingleZombie()
{
	//产生一个box
	//GetWorld()->SpawnActor()
}

