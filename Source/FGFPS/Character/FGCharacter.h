#pragma once

#include "GameFramework/Character.h"
#include "Interfaces/FGWeaponInterface.h"
#include "FGCharacter.generated.h"

class UInputComponent;

UCLASS()
class AFGCharacter : public ACharacter, public IFGWeaponInterface
{
	GENERATED_BODY()
public:
	AFGCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	virtual void Tick(float DeltaSeconds) override;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	bool GetShootDirection_Implementation(FVector& StartLocation, FVector& ForwardDirection) const;
	bool GetSpreadMultiplier_Implementation(float& SpreadMultiplier) const;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "FG|Character")
	void EquipNewWeapon(class UFGWeaponSchematic* NewWeaponSchematic);

	UFUNCTION(BlueprintPure, Category = "FG|Character")
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintPure, Category = "FG|Character")
	FORCEINLINE AFGWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	FORCEINLINE class UFGLuaComponent* GetLuaComponent() const { return LuaComponent; }

	class UFGLuaContext* GetLuaContext() const;

	bool IsCharacterMoving() const;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Lua)
	class UFGLuaComponent* LuaComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = Interaction)
	class UFGPlayerInteractorComponent* InteractorComponent = nullptr;

	UFUNCTION()
	void HandleWeaponFire();

	UPROPERTY(Transient)
	class AFGWeapon* CurrentWeapon = nullptr;

	UPROPERTY(Transient)
	class UFGWeaponSchematic* WeaponSchematic = nullptr;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

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

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);
	
protected:
	

};

