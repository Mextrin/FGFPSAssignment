// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FGGameState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
	enum class States : uint8 { PreLaunch, Running, Won, Lost };

UCLASS()
class FGFPS_API AFGGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
		States GetState();

	UFUNCTION(BlueprintCallable)
		void StartGame();

	UFUNCTION(BlueprintCallable)
		void EndGame(bool hasWon);

	UFUNCTION(BlueprintCallable)
		void SetPause(bool PauseMode);
	UFUNCTION(BlueprintCallable)
		bool GetPauseState();

	UPROPERTY(EditDefaultsOnly, Category = "Post Game")
		TSubclassOf<class UUserWidget> VictoryScreen;

	UPROPERTY(EditDefaultsOnly, Category = "Post Game")
		TSubclassOf<class UUserWidget> DefeatScreen;

private:
	States CurrentState = States::PreLaunch;
	bool IsGamePaused = false;
};
