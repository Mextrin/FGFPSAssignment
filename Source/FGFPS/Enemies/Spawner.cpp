// Fill out your copyright notice in the Description page of Project Settings.

#include "Spawner.h"
#include "FGEnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "../../../../../Program Files/Epic Games/UE_4.21/Engine/Source/Runtime/Engine/Classes/Engine/World.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpawnRange = CreateDefaultSubobject<USphereComponent>("Range sphere");
	RootComponent = SpawnRange;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	SpawnRange->OnComponentEndOverlap.AddDynamic(this, &ASpawner::OnSpawnerExit);
	Super::BeginPlay();
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanSpawn)
	{
		CanSpawn = false;

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AFGEnemyCharacter>(EnemyTypes[0], GetActorLocation(), GetActorRotation(), SpawnInfo);
	}
}

void ASpawner::OnSpawnerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AFGEnemyCharacter::StaticClass()))
	{
		CanSpawn = true;
	}
}

