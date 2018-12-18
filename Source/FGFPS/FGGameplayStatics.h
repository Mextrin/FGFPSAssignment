#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FGGameplayStatics.generated.h"

UCLASS()
class FGFPS_API UFGGameplayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "FG|Lua", meta = (WorldContext = "WorldContextObject"))
	static class UFGLuaComponent* GetLuaComponentFromActor(UObject* WorldContextObject, class AActor* TargetActor, bool bUseFindComponent = true);

};
