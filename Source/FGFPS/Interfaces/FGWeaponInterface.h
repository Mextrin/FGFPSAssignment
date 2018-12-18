#pragma once

#include "UObject/Interface.h"
#include "CoreMinimal.h"
#include "FGWeaponInterface.generated.h"

UINTERFACE()
class FGFPS_API UFGWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class FGFPS_API IFGWeaponInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FG|Interface")
	class AFGWeapon* GetWeaponInstance() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FG|Interface")
	bool GetShootDirection(FVector& StartLocation, FVector& ForwardDirection) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FG|Interface")
	bool GetSpreadMultiplier(float& SpreadMultiplier) const;
};
