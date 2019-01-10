#include "WinLose.h"
#include "Kismet/GameplayStatics.h"
#include "UserWidget.h"

void UFGWinLose::NativeConstruct()
{
	Super::NativeConstruct();
}

void UFGWinLose::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName(), false));
}

void UFGWinLose::ExitGame()
{
	FGenericPlatformMisc::RequestExit(false);
}

