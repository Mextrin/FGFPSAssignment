// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnerNode.h"
#include "Components/SphereComponent.h"
#include "FGEnemyCharacter.h"

// Sets default values
ASpawnerNode::ASpawnerNode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnRange = CreateDefaultSubobject<USphereComponent>("SphereRange");
	SpawnRange->OnComponentEndOverlap.AddDynamic(this, &ASpawnerNode::OnEnemyExitRange);

	RootComponent = SpawnRange;
}

// Called when the game starts or when spawned
void ASpawnerNode::BeginPlay()
{
	Super::BeginPlay();


}

bool ASpawnerNode::Spawn(TSubclassOf<AFGEnemyCharacter> EnemyType)
{
	if (CanSpawn)
	{
		GetWorld()->SpawnActor<AFGEnemyCharacter>(EnemyType, GetActorLocation(), GetActorRotation());
		CanSpawn = false;
		return true;
	}

	return false;
}

void ASpawnerNode::OnEnemyExitRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AFGEnemyCharacter::StaticClass()))
	{
		CanSpawn = true;
	}
}