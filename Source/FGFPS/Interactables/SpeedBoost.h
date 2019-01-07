#pragma once

#include "Components/FGInteractableComponent.h"
#include "SpeedBoost.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class FGFPS_API AFGSpeedBoost : public AActor
{
	GENERATED_BODY()
public:
	AFGSpeedBoost(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(VisibleDefaultsOnly)
	UFGInteractableComponent* Interactable;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	void OnSuccessInteract();
};
