// Fill out your copyright notice in the Description page of Project Settings.

#include "GameController.h"
#include "Engine/Engine.h"

AGameController::AGameController()
{
	
}

void AGameController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AGameController::StartWave()
{
	WaveCurrent++;

}
