// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MyActor_Spawner.generated.h"

UCLASS()
class TOPDOWN_API AMyActor_Spawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor_Spawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	UFUNCTION(BlueprintCallable, Category = "ZombieSpawner")
	void SpawnSingleZombie();
	int Zombie_num;

	void Init();

};
