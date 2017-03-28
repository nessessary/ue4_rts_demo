// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDown.h"
#include "engine.h"
#include "TopDownCharacter.h"
#include "MyActor_Spawner.h"
#include "BuildCamp.h"

// Sets default values
AMyActor_Spawner::AMyActor_Spawner()
{
	static int i = 0;
	Zombie_num = 0;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"), false);

	auto component = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"), false);
	component->SetupAttachment(RootComponent);

	//auto mesh = CreateDefaultSubobject<UMeshComponent>(TEXT("MyMesh"), false);
	//mesh->SetupAttachment(component);
	if (i < 1) {
		i++;
	}
	Init();

}

// Called when the game starts or when spawned
void AMyActor_Spawner::BeginPlay()
{
	Super::BeginPlay();
	FString str = FString::Printf(TEXT("AMyActor_Spawner::BeginPlay"));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, str);

	// 创建一个定时器
	//FTimerHandle SpawnTimer;
	//GetWorldTimerManager().SetTimer(SpawnTimer, this, &AMyActor_Spawner::SpawnSingleZombie, 2, true);
}

// Called every frame
void AMyActor_Spawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AMyActor_Spawner::SpawnSingleZombie()
{
	//产生..一个box
	//GetWorld()->SpawnActor()
	FVector NewLocation = GetActorLocation() + FVector(0.f, 0.f, 1.f);
	//UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), NewLocation.X, NewLocation.Y, NewLocation.Z );

	// Spawn the new actor (Using GetClass() instead of AMySpawner so that if someone derives a new class  
	// from AMySpawner we spawn an instance of that class instead)  
	//auto NewActor = GetWorld()->SpawnActor<AMyActor_Spawner>(GetClass(), NewLocation, FRotator::ZeroRotator);
	//GetAllActorsOfClass

	// 直接创建蓝图类
	UClass* p1 = LoadClass<ACharacter>(NULL, TEXT("/Game/bluprint/Character_Blueprint.Character_Blueprint_C"));
	

	UClass* p = LoadClass<ATopDownCharacter>(NULL, TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter.TopDownCharacter_C"));	
	if (p && Zombie_num<3) {
		//NewObject<ACharacter>(p);
		//ConstructObject<UObject>(p);
		// can't use p->GetClass()
		ATopDownCharacter* pCharacter = GetWorld()->SpawnActor<ATopDownCharacter>(p, NewLocation, FRotator::ZeroRotator);
		Zombie_num++;
	}
}

void AMyActor_Spawner::Init()
{
	// x = up(+),down(-)
	// y = left(-), right(+)
	if(GetWorld())
		GetWorld()->SpawnActor<ABuildCamp>(ABuildCamp::StaticClass(), FVector(-700, -400, 200), FRotator::ZeroRotator);
}

