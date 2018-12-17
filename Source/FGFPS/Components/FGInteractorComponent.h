#pragma once

#include "Components/ActorComponent.h"
#include "FGInteractorComponent.generated.h"

UCLASS(Abstract)
class FGFPS_API UFGInteractorComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UFGInteractorComponent(const FObjectInitializer& ObjectInitializer);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartInteracting();
	void StopInteracting();

	virtual void GetLookDirection(FVector& StartLocation, FVector& ForwardDirection) const;
	virtual float GetInteractionSpeed() const { return 1.0f; }

	FORCEINLINE bool IsInteracting() const { return bIsInteracting; }

protected:

	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	float InteractionLength = 100.0f;

	UPROPERTY(Transient)
	class UFGInteractableComponent* CurrentInteractable = nullptr;

	UPROPERTY(Transient)
	class UFGInteractionWidget* InteractionWidget = nullptr;

	bool bIsInteracting = false;
};
