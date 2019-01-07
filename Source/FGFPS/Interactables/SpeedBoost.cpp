#include "SpeedBoost.h"
#include "Components/FGInteractableComponent.h"
#include "Components/StaticMeshComponent.h"

AFGSpeedBoost::AFGSpeedBoost(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Interactable = ObjectInitializer.CreateDefaultSubobject<UFGInteractableComponent>(this, TEXT("InteractableComponent"));
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = Mesh;
}

void AFGSpeedBoost::OnSuccessInteract()
{
	
}