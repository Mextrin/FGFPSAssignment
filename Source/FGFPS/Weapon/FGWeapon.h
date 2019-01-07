#pragma once

#include "GameFramework/Actor.h"
#include "Interfaces/FGLuaComponentInterface.h"
#include "FGWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFGOnFireDelegate);

UCLASS()
class FGFPS_API AFGWeapon : public AActor
{
	GENERATED_BODY()
public:

	AFGWeapon(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void Fire();

	void Reload();

	UPROPERTY(BlueprintAssignable, Category = "FG|Weapon")
	FFGOnFireDelegate OnFire;

	class UFGLuaComponent* GetLuaComponent_Implementation() const;

	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	UFUNCTION(BlueprintCallable, Category = "FG|Weapon")
	void GetCurrentAmmo();
	UFUNCTION(BlueprintCallable, Category = "FG|Weapon")
	void GetMaxAmmo();

	UPROPERTY(BlueprintReadWrite, Category = "FG|Weapon")
	int CurrtAmmo = 0;
	UPROPERTY(BlueprintReadWrite, Category = "FG|Weapon")
	int MaxAmmo = 0;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class UFGLuaComponent* LuaComponent = nullptr;
};
