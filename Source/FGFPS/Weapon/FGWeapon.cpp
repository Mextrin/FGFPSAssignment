#include "FGWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Engine/CollisionProfile.h"
#include "Interfaces/FGWeaponInterface.h"
#include "Components/FGLuaComponent.h"

#include "lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FGPlayerCharacter.h"

using namespace luabridge;

AFGWeapon::AFGWeapon(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	LuaComponent = ObjectInitializer.CreateDefaultSubobject<UFGLuaComponent>(this, TEXT("LuaComponent"));
}

void AFGWeapon::BeginPlay()
{
	Super::BeginPlay();
	LuaComponent->CallFunction("BeginPlay");
	CurrtAmmo = LuaComponent->GetNumber("CurrentAmmo");
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFGPlayerCharacter::StaticClass(), Players);
	AnotherCharWeapon = Cast<AFGPlayerCharacter>(Players[0]);
	// TODO: How to export a class and then a function on that class using luabridge
	//getGlobalNamespace(LuaComponent->GetLuaState())
	//	.beginClass<AFGWeapon>("Weapon")
	//	.addFunction("Fire", &AFGWeapon::Fire)
	//	.endClass();

	//push(LuaComponent->GetLuaState(), this);
	//lua_setglobal(LuaComponent->GetLuaState(), "MyWeapon");
}

void AFGWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	LuaComponent->CallFunction_OneParamNumber("Tick", DeltaSeconds);
	InfiniteAmmo = LuaComponent->GetBool("InfiniteAmmo");
	//Debug print on screen ammount of current ammo
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AmountRemoved = %d"),AmmoToAdd ));
}

void AFGWeapon::Fire()
{
	if (CurrtAmmo > 0)
	{
		FVector StartLocation = GetActorLocation();
		FVector ForwardDirection = GetActorForwardVector();

		float Spread = LuaComponent->GetNumber(TEXT("Spread"));

		if (GetOwner()->GetClass()->ImplementsInterface(UFGWeaponInterface::StaticClass()))
		{
			IFGWeaponInterface::Execute_GetShootDirection(GetOwner(), StartLocation, ForwardDirection);

			float Multiplier = 1.0f;
			IFGWeaponInterface::Execute_GetSpreadMultiplier(GetOwner(), Multiplier);
			Spread *= Multiplier;
		}

		TArray<AActor*> IgnoreActors{ this, GetOwner() };

		const float HorizontalConeHalfAngleRad = FMath::DegreesToRadians(Spread != 0.0f ? Spread : 1.0f);
		ForwardDirection = FMath::VRandCone(ForwardDirection, HorizontalConeHalfAngleRad);

		const FVector EndLocation = StartLocation + (ForwardDirection * 10000.0f); // 10000.0f Magic number, how far we are testing

		FHitResult Hit;
		if (UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), StartLocation, EndLocation, UCollisionProfile::BlockAllDynamic_ProfileName, false, IgnoreActors, EDrawDebugTrace::ForDuration, Hit, true))
		{
			if (Hit.Actor.IsValid()) // Formality
			{
				float Damage = LuaComponent->GetNumber(TEXT("Damage"));

				FPointDamageEvent DamageEvent;
				DamageEvent.Damage = Damage;
				DamageEvent.HitInfo = Hit;
				DamageEvent.ShotDirection = ForwardDirection;

				Hit.Actor->TakeDamage(Damage, DamageEvent, (Instigator ? Instigator->GetController() : nullptr), this);
			
			}
		}

		if (!InfiniteAmmo)
		{
			CurrtAmmo--;
			LuaComponent->SetNumber(CurrtAmmo, "CurrentAmmo");
		}
		OnFire.Broadcast();
	}
}

void AFGWeapon::Reload()
{
	if (!InfiniteAmmo)
	{
		LuaComponent->CallFunction("Reload");
		CurrtAmmo = LuaComponent->GetNumber("CurrentAmmo");
	}
}

void AFGWeapon::RemoveAmmo()
{
	LuaComponent->CallFunction("RemoveAmmo");
	AmmoToAdd = LuaComponent->GetNumber("RemovedAmmo");
}

void AFGWeapon::AddAmmoToTurret()
{
	CurrtAmmo = AnotherCharWeapon->CurrentWeapon->AmmoToAdd;
	LuaComponent->SetNumber(CurrtAmmo, "CurrentAmmo");
}

UFGLuaComponent* AFGWeapon::GetLuaComponent_Implementation() const
{
	return LuaComponent;
}

void AFGWeapon::SetInfiniteAmmo()
{
	LuaComponent->CallFunction("SetInfiniteAmmo");
}

void AFGWeapon::SpreadBoost()
{
	LuaComponent->CallFunction("DecreaseSpread");
}

void AFGWeapon::DamageBoost()
{
	LuaComponent->CallFunction("DamageBoost");
}

void AFGWeapon::AddAmmo()
{
	LuaComponent->CallFunction("AddAmmo");
}

