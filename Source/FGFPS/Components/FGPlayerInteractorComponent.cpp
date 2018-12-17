#include "FGPlayerInteractorComponent.h"
#include "Character/FGCharacter.h"
#include "Camera/CameraComponent.h"

void UFGPlayerInteractorComponent::GetLookDirection(FVector& StartLocation, FVector& ForwardDirection) const
{
	AFGCharacter* PlayerCharacter = GetCharacterOwner();
	StartLocation = PlayerCharacter->GetFirstPersonCameraComponent()->GetComponentLocation();
	ForwardDirection = PlayerCharacter->GetFirstPersonCameraComponent()->GetForwardVector();
}

float UFGPlayerInteractorComponent::GetInteractionSpeed() const
{
	return 3.0f;
}

AFGCharacter* UFGPlayerInteractorComponent::GetCharacterOwner() const
{
	return CastChecked<AFGCharacter>(GetOwner());
}

