#pragma once

#include "UserWidget.h"
#include "WinLose.generated.h"


UCLASS()
class FGFPS_API UFGWinLose : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void RestartGame();

	UFUNCTION(BlueprintCallable)
	void ExitGame();

};
