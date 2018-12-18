#include "FGCharacterMovementComponent.h"
#include "Character/FGCharacter.h"
#include "Components/FGLuaComponent.h"
#include "Lua/FGLuaContext.h"

void UFGCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = CastChecked<AFGCharacter>(GetOwner());
}

float UFGCharacterMovementComponent::GetMaxSpeed() const
{
	if (OwnerCharacter->GetLuaContext()->DoesFunctionExist("GetSpeedMultiplier"))
	{
		return Super::GetMaxSpeed() * OwnerCharacter->GetLuaContext()->CallFunction_RetValueNumber("GetSpeedMultiplier", 1.0f);
	}

	return Super::GetMaxSpeed();
}
