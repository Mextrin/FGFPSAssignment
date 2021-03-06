// Fill out your copyright notice in the Description page of Project Settings.

#include "GameController.h"
#include "Engine/Engine.h"
#include "Enemies/Spawner.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "FGGameState.h"

AGameController::AGameController()
{
	
}

void AGameController::BeginPlay()
{
	TArray<AActor*> SpawnerActors;	//Array form in case multiple spawners are requested. (eg. spawners for specific types of enemies)
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawner::StaticClass(), SpawnerActors);

	for (int i = 0; i < SpawnerActors.Num(); i++)
	{
		SpawnerList.Add(CastChecked<ASpawner>(SpawnerActors[i]));
	}

	CurrentGameState = CastChecked<AFGGameState>(GameState);
}

void AGameController::Tick(float DeltaTime)
{
	if (CurrentGameState->GetState() == States::Running)
	{
		StartWave();
	}
}


void AGameController::StartWave()
{
	if (!WaveInProgress)
	{
		WaveInProgress = true;
		WaveCurrent++;

		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, this, &AGameController::DoSpawn, 5.0f);
	}
}

void AGameController::DoSpawn()
{
	SpawnerList[0]->SpawnEnemies(EnemiesPerWave);

	EnemyCount += EnemiesPerWave;
}

void AGameController::EnemyDecrease()
{
	EnemyCount--;
	
	if (EnemyCount <= 0)
	{
		WaveInProgress = false;
		if (WaveCurrent >= WaveAmount)
		{
			CurrentGameState->EndGame(true);
		}
	}
}

void AGameController::EnemyTargetReached()
{
	EnemiesToFail--;
	EnemyDecrease();

	if (EnemiesToFail < 0) EnemiesToFail = 0;
	if (EnemiesToFail <= 0)
	{
		CurrentGameState->EndGame(false);
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("Game Lost"));

		//Disable player input.
		//APlayerController* PlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		//PlayerController->DisableInput(PlayerController);

		DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		//Show Defeat UI.
		//Restart after a couple seconds.
	}
}
