// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"
#include "FGEnemyCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	spawnRange = CreateDefaultSubobject<USphereComponent>("Range sphere");
	RootComponent = spawnRange;
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

}

