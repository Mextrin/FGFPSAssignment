#include "FGCharacterMovementComponent.h"
#include "Character/FGCharacter.h"
#include "Components/FGLuaComponent.h"

float UFGCharacterMovementComponent::GetMaxSpeed() const
{
	return Super::GetMaxSpeed() * GetOwnerFGCharacter()->GetLuaComponent()->CallFunction_RetValueNumber(TEXT("GetSpeedMultiplier"), 1.0f);
}

class AFGCharacter* UFGCharacterMovementComponent::GetOwnerFGCharacter() const
{
	return CastChecked<AFGCharacter>(GetOwner());
}
