#pragma once

#include "UserWidget.h"
#include "FGInteractionWidget.generated.h"

UCLASS()
class FGFPS_API UFGInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "FG|UI")
	void OnSetProgress(float Progress);

	UFUNCTION(BlueprintImplementableEvent, Category = "FG|UI")
	void OnSetIsInteracting(bool bIsInteracting);
};
