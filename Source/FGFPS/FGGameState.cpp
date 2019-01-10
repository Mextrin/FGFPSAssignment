// Fill out your copyright notice in the Description page of Project Settings.

#include "FGGameState.h"
#include "../../../../Program Files/Epic Games/UE_4.21/Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "../../../../Program Files/Epic Games/UE_4.21/Engine/Source/Runtime/UMG/Public/Blueprint/UserWidget.h"

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
	APlayerController* const PlayerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	UUserWidget* Widget;
	if (hasWon)
	{
		CurrentState = States::Won;
		Widget = CreateWidget<UUserWidget>(PlayerController, VictoryScreen);
	}
	else
	{
		CurrentState = States::Lost;
		Widget = CreateWidget<UUserWidget>(PlayerController, DefeatScreen);
	}

	if (Widget)
	{
		//Show Victory widget
		Widget->AddToViewport();
	}

	PlayerController->bShowMouseCursor = true;
}

void AFGGameState::SetPause(bool PauseMode)
{
	IsGamePaused = PauseMode;
	UGameplayStatics::SetGamePaused(GetWorld(), IsGamePaused);
}

bool AFGGameState::GetPauseState()
{
	return IsGamePaused;
}
