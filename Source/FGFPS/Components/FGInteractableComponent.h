#pragma once

#include "Components/BoxComponent.h"
#include "FGInteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFGOnInteractionSuccessful, class UFGInteractableComponent*, Interactable, class UFGInteractorComponent*, Interactor);

UCLASS(meta = (BlueprintSpawnableComponent))
class FGFPS_API UFGInteractableComponent : public UBoxComponent
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable, Category = "FG|Interaction")
	FFGOnInteractionSuccessful OnInteractionSuccessful;

	bool Interact(class UFGInteractorComponent* Interactor);

	void StartInteraction();
	void StopInteraction();

	FORCEINLINE float GetProgress() const { return InteractionProgress / InteractionTime; }
	FORCEINLINE TSubclassOf<class UFGInteractionWidget> GetInteractionWidget() const { return InteractionWidget; }

private:
	// How long the interaction is
	UPROPERTY(EditAnywhere, Category = Interaction)
	float InteractionTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = Interaction)
	TSubclassOf<class UFGInteractionWidget> InteractionWidget;

	float InteractionProgress = 0.0f;
};
