#pragma once

#include "FGInteractorComponent.h"
#include "FGPlayerInteractorComponent.generated.h"

UCLASS()
class FGFPS_API UFGPlayerInteractorComponent : public UFGInteractorComponent
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;

	virtual void GetLookDirection(FVector& StartLocation, FVector& ForwardDirection) const override;
	virtual float GetInteractionSpeed() const override;

private:
	UPROPERTY(Transient)
	class AFGPlayerCharacter* PlayerOwner = nullptr;
};
