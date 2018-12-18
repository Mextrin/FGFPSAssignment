#include "FGPlayerInteractorComponent.h"
#include "Player/FGPlayerCharacter.h"
#include "Camera/CameraComponent.h"

void UFGPlayerInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerOwner = CastChecked<AFGPlayerCharacter>(GetOwner());
}

void UFGPlayerInteractorComponent::GetLookDirection(FVector& StartLocation, FVector& ForwardDirection) const
{
	StartLocation = PlayerOwner->GetFirstPersonCameraComponent()->GetComponentLocation();
	ForwardDirection = PlayerOwner->GetFirstPersonCameraComponent()->GetForwardVector();
}

float UFGPlayerInteractorComponent::GetInteractionSpeed() const
{
	return 3.0f;
}
