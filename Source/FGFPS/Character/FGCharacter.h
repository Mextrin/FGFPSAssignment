#pragma once

#include "GameFramework/Character.h"
#include "FGCharacter.generated.h"

UCLASS()
class AFGCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	AFGCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "FG|Character")
	virtual void EquipNewWeapon(class UFGWeaponSchematic* NewWeaponSchematic, FName SocketName);

	UFUNCTION(BlueprintPure, Category = "FG|Character")
	FORCEINLINE AFGWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	FORCEINLINE class UFGLuaComponent* GetLuaComponent() const { return LuaComponent; }

	class UFGLuaContext* GetLuaContext() const;

	bool IsCharacterMoving() const;

	FORCEINLINE class UFGWeaponSchematic* GetWeaponSchemtic() const { check(WeaponSchematic != nullptr); return WeaponSchematic; }

	UFUNCTION()
	virtual void HandleWeaponFire();

	UPROPERTY(Transient)
	class AFGWeapon* CurrentWeapon = nullptr;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Lua)
	class UFGLuaComponent* LuaComponent = nullptr;


	UPROPERTY(Transient)
	class UFGWeaponSchematic* WeaponSchematic = nullptr;

};

