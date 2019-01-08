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
		int waveAmount = 10;

	UPROPERTY(EditAnywhere)
		int waveCurrent = 1;

	UPROPERTY(EditAnywhere)
		int enemiesPerWave = 15;

	UPROPERTY()
		int enemiesAlive = 0;

protected:
	
};
