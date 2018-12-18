#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "FGCharacterMovementComponent.generated.h"

UCLASS()
class FGFPS_API UFGCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	virtual float GetMaxSpeed() const override;

private:
	UPROPERTY(Transient)
	class AFGCharacter* OwnerCharacter = nullptr;
};
