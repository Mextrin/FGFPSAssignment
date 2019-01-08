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

	virtual void Tick(float DeltaTime) override;

	void StartWave();
	
	UPROPERTY(EditAnywhere)
		int WaveAmount = 10;

	UPROPERTY(EditAnywhere)
		int WaveCurrent = 0;

	UPROPERTY(EditAnywhere)
		int EnemiesPerWave = 15;

	UPROPERTY()
		int EnemiesAlive = 0;

	UPROPERTY(EditDefaultsToFail)
		int EnemiesToFail = 10;

protected:
	
};
