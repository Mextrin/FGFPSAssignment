#pragma once

#include "Character/FGCharacter.h"
#include "Interfaces/FGWeaponInterface.h"
#include "FGPlayerCharacter.generated.h"

UCLASS()
class FGFPS_API AFGPlayerCharacter : public AFGCharacter, public IFGWeaponInterface
{
	GENERATED_BODY()
public:
	AFGPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, Category = "FG|PlayerCharacter")
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UFUNCTION(BlueprintPure, Category = "FG|PlayerCharacter")
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	UFUNCTION(BlueprintPure, Category = "FG|PlayerCharacter")
	FORCEINLINE class UFGPlayerInteractorComponent* GetInteractorComponent() const { return InteractorComponent; }

	bool GetShootDirection_Implementation(FVector& StartLocation, FVector& ForwardDirection) const;
	bool GetSpreadMultiplier_Implementation(float& SpreadMultiplier) const;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void EquipNewWeapon(class UFGWeaponSchematic* NewWeaponSchematic, FName SocketName) override;

protected:

	virtual void HandleWeaponFire() override;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Interaction)
	class UFGPlayerInteractorComponent* InteractorComponent = nullptr;

	FTransform DefaultTransform;
	FTransform TargetTransform;

	bool bDoADS = false;

	void SetWeaponADS();


	void ADSPressed();

	void ADSReleased();

	void SprintPressed();

	void SprintReleased();

	void InteractPressed();

	void InteractReleased();

	void FirePressed();

	void MoveForward(float Val);

	void MoveRight(float Val);
};
