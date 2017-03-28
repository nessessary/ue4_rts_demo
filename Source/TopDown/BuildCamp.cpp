// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDown.h"
#include "BuildCamp.h"
#include "TopDownCharacter.h"

// Sets default values
ABuildCamp::ABuildCamp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Add Build Portion
	auto Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	Mesh->SetupAttachment(RootComponent);
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (MeshAsset.Object != nullptr)
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void ABuildCamp::BeginPlay()
{
	Super::BeginPlay();
	
	// 创建一个定时器
	FTimerHandle SpawnTimer;
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ABuildCamp::SpawnSingleZombie, 2, true);
}

// Called every frame
void ABuildCamp::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABuildCamp::SpawnSingleZombie()
{
	FVector NewLocation = GetActorLocation() + FVector(50, 0.f, 1.f);
	UClass* p = LoadClass<ATopDownCharacter>(NULL, TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter.TopDownCharacter_C"));
	if (p && Zombie_num_ < 3) {
		ATopDownCharacter* pCharacter = GetWorld()->SpawnActor<ATopDownCharacter>(p, NewLocation, FRotator::ZeroRotator);
		pCharacter->PlayAI->Possess(pCharacter);
		pCharacter->PossessedBy(pCharacter->PlayAI);
		pCharacter->PlayAI->MoveTo(NewLocation + FVector(400, 10, 0));
		Zombie_num_++;
	}
}

