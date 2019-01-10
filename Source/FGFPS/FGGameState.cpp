// Fill out your copyright notice in the Description page of Project Settings.

#include "FGGameState.h"

States AFGGameState::GetState()
{
	return CurrentState;
}

void AFGGameState::StartGame()
{
	CurrentState = States::Running;
}

void AFGGameState::EndGame()
{
	CurrentState = States::Finished;
}
