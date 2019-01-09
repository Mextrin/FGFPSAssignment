// Fill out your copyright notice in the Description page of Project Settings.

#include "GameController.h"
#include "Engine/Engine.h"
#include "Enemies/Spawner.h"
#include "Kismet/GameplayStatics.h"

AGameController::AGameController()
{
	
}

void AGameController::BeginPlay()
{
	TArray<AActor*> SpawnerActors;	//Array form in case multiple spawners are requested. (eg. spawners for specific types of enemies)
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawner::StaticClass(), SpawnerActors);

	for (int i = 0; i < SpawnerActors.Num(); i++)
	{
		SpawnerList.Add(Cast<ASpawner>(SpawnerActors[i]));
	}
}

void AGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameController::StartWave()
{
	if (!WaveInProgress)
	{
		WaveInProgress = true;
		WaveCurrent++;

		int enemiesToSpawn = 15;
		SpawnerList[0]->SpawnEnemies(enemiesToSpawn);

		EnemyCount += enemiesToSpawn;
	}
}

void AGameController::EnemyDecrease()
{
	EnemyCount--;
	
	if (EnemyCount <= 0)
	{
		WaveInProgress = false;
	}
}

void AGameController::EnemyTargetReached()
{
	EnemiesToFail--;
	EnemyDecrease();

	if (EnemiesToFail <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("Game Lost"));

		//Disable player input.
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		PlayerController->DisableInput(PlayerController);

		//Show Defeat UI.
		//Restart after a couple seconds.
	}
}
