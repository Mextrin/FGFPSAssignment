// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"
#include "FGEnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "../../../../../Program Files/Epic Games/UE_4.21/Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "SpawnerNode.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ToSpawn > 0)
	{
		for (int i = 0; i < SpawnNodes.Num(); i++)
		{
			bool Success = SpawnNodes[i]->Spawn(EnemyTypes[0]);

			if (Success) ToSpawn--;
		}
	}
}

void ASpawner::SpawnEnemies(int Amount)
{
	ToSpawn += Amount;
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Added %d enemies to spawn"), Amount));
}
