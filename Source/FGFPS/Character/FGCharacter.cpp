#include "FGCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Weapon/FGWeapon.h"
#include "Weapon/FGWeaponSchematic.h"
#include "Kismet/GameplayStatics.h"
#include "Components/FGCharacterMovementComponent.h"
#include "Components/FGLuaComponent.h"
#include "Lua/FGLuaContext.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

AFGCharacter::AFGCharacter(const FObjectInitializer& ObjectInitializer) : 
	Super( ObjectInitializer.SetDefaultSubobjectClass<UFGCharacterMovementComponent>(ACharacter::CharacterMovementComponentName) )
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	LuaComponent = CreateDefaultSubobject<UFGLuaComponent>(TEXT("LuaComponent"));
}

void AFGCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetLuaContext()->IsLoaded())
	{
		GetLuaContext()->SetBool(IsCharacterMoving(), "IsMoving"); // Even if IsMoving does not exist in our script, calling this will create it.
		if (GetLuaContext()->DoesFunctionExist("Tick"))
		{
			GetLuaContext()->CallFunction("Tick", DeltaSeconds);
		}
	}
}

void AFGCharacter::EquipNewWeapon(UFGWeaponSchematic* NewWeaponSchematic, FName SocketName)
{
	// Don't crash but notify programmers that we are calling this function without having it overridden in parent class
	ensureMsgf(false, TEXT("Function not overridden in parent class"));
}

UFGLuaContext* AFGCharacter::GetLuaContext() const
{
	return GetLuaComponent()->GetLuaContext();
}

bool AFGCharacter::IsCharacterMoving() const
{
	return GetCharacterMovement()->IsMovingOnGround() && GetCharacterMovement()->Velocity.Size2D() > KINDA_SMALL_NUMBER;
}

void AFGCharacter::HandleWeaponFire()
{
	if (!WeaponSchematic)
	{
		return;
	}

	if (GetWeaponSchemtic()->ShootSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GetWeaponSchemtic()->ShootSound, GetActorLocation());
	}
}
