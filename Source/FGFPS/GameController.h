// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameController.generated.h"

/**
 * 
 */
UCLASS()
class FGFPS_API AGameController : public AGameMode
{
	GENERATED_BODY()

public:
	AGameController();


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void StartWave();
		
	void DoSpawn();

	UFUNCTION(BlueprintCallable)
		void EnemyDecrease();

	UFUNCTION(BlueprintCallable)
		void EnemyTargetReached();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int WaveAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int WaveCurrent = 0;

	UPROPERTY(EditAnywhere)
		int EnemiesPerWave = 15;

	UPROPERTY()
		int EnemyCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int EnemiesToFail = 5;

	TArray<class ASpawner*> SpawnerList;
	bool WaveInProgress = false;

protected:
	
};
