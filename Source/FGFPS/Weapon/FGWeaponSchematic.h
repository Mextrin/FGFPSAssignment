#pragma once

#include "Engine/DataAsset.h"
#include "FGWeaponSchematic.generated.h"

UCLASS()
class FGFPS_API UFGWeaponSchematic : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = Actor)
	TSubclassOf<class AFGWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	class UAnimMontage* ShootMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Audio)
	class USoundBase* ShootSound = nullptr;
};
