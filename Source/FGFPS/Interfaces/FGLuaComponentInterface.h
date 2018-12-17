#pragma once

#include "UObject/Interface.h"
#include "FGLuaComponentInterface.generated.h"

UINTERFACE()
class FGFPS_API UFGLuaComponentInterface : public UInterface
{
	GENERATED_BODY()
};

class FGFPS_API IFGLuaComponentInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Gameplay)
	class UFGLuaComponent* GetLuaComponent() const;
};
