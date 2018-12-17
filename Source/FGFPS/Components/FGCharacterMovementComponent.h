#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "FGCharacterMovementComponent.generated.h"

UCLASS()
class FGFPS_API UFGCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	virtual float GetMaxSpeed() const override;

private:
	class AFGCharacter* GetOwnerFGCharacter() const;
};
