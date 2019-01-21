// Fill out your copyright notice in the Description page of Project Settings.

#include "FGGameState.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

States AFGGameState::GetState()
{
	return CurrentState;
}

void AFGGameState::StartGame()
{
	CurrentState = States::Running;
}

void AFGGameState::EndGame(bool hasWon)
{
	CurrentState = States::Finished;
	APlayerController* const PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	UUserWidget* Widget;
	if (hasWon)
	{
		Widget = CreateWidget<UUserWidget>(PlayerController, VictoryScreen);
	}
	else
	{
		Widget = CreateWidget<UUserWidget>(PlayerController, DefeatScreen);
	}

	if (Widget)
	{
		//Show Victory widget
		Widget->AddToViewport();
	}

	SetPause();
	PlayerController->bShowMouseCursor = true;
}

void AFGGameState::SetPause(bool PauseMode)
{

	if (CurrentState == States::Finished)	//Pause anyway
	{
		IsGamePaused = true;
	}
	else
	{
		IsGamePaused = PauseMode;
	}

	UGameplayStatics::SetGamePaused(GetWorld(), IsGamePaused);
}

bool AFGGameState::GetPauseState()
{
	return IsGamePaused;
}
