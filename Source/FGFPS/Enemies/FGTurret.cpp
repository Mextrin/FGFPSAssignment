#include "FGTurret.h"
#include "Components/StaticMeshComponent.h"
#include "Components/FGLuaComponent.h"
#include "Components/FGInteractableComponent.h"
#include "Weapon/FGWeapon.h"
#include "Weapon/FGWeaponSchematic.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Character/FGCharacter.h"
#include "TimerManager.h"

AFGTurret::AFGTurret(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = Mesh;

	LuaComponent = ObjectInitializer.CreateDefaultSubobject<UFGLuaComponent>(this, TEXT("Lua"));

	Interactable = ObjectInitializer.CreateDefaultSubobject<UFGInteractableComponent>(this, TEXT("Interactable"));
	Interactable->SetupAttachment(Mesh);

	WeaponAttachPoint = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Scene"));
	WeaponAttachPoint->SetupAttachment(Mesh);
}

void AFGTurret::BeginPlay()
{
	Super::BeginPlay();

	CachedCollisionShape = FCollisionShape::MakeSphere(VisibilityRange);
	CachedCollisionQueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(SphereOverlapComponents), false);

	for (auto Iter = ObjectTypes.CreateConstIterator(); Iter; ++Iter)
	{
		const ECollisionChannel & Channel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, *Iter);
		CachedCollisionObjectParams.AddObjectTypesToQuery(Channel);
	}

	OverlapDelegate.BindUObject(this, &AFGTurret::HandleAsyncOverlap);

	StartDetection();
}

void AFGTurret::Destroyed()
{
	StopDetection();

	Super::Destroyed();
}

void AFGTurret::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!CurrentWeapon)
	{
		return;
	}

	WeaponAttachPoint->AddLocalRotation(FRotator(0.0f, LuaComponent->GetNumber("RotationSpeed"), 0.0f), false, nullptr, ETeleportType::TeleportPhysics);

	LuaComponent->CallFunction_OneParamNumber(TEXT("Tick"), DeltaSeconds);

	if (LuaComponent->CallFunction_RetValueBool(TEXT("CanWeaponShoot")))
	{
		Fire();
		LuaComponent->CallFunction(TEXT("OnShoot"));
	}

	if (TargetActor)
	{
		const FVector Direction = (TargetActor->GetActorLocation() - CurrentWeapon->GetActorLocation()).GetSafeNormal();
		CurrentWeapon->SetActorRotation(Direction.Rotation());
	}

	/*
	TArray<FOverlapResult> Overlaps;
	if (GetWorld()->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat::Identity, CachedCollisionObjectParams, CachedCollisionShape, CachedCollisionQueryParams))
	{
		for ( FOverlapResult& Overlap : Overlaps)
		{
			TargetActor = Overlap.Actor.Get();
			break;
		}
	}
	*/
}

bool AFGTurret::GetShootDirection_Implementation(FVector& StartLocation, FVector& ForwardDirection) const
{
	StartLocation = FVector::ZeroVector;
	ForwardDirection = FVector::ForwardVector;

	if (!CurrentWeapon)
	{
		return false;
	}

	StartLocation = CurrentWeapon->GetWeaponMesh()->GetSocketTransform(TEXT("Muzzle")).GetLocation();
	ForwardDirection = CurrentWeapon->GetWeaponMesh()->GetSocketTransform(TEXT("Muzzle")).GetRotation().Vector();

	return true;
}

void AFGTurret::CreateWeapon(class UFGWeaponSchematic* NewWeaponSchematic)
{
	if (!NewWeaponSchematic || !NewWeaponSchematic->WeaponClass)
	{
		return;
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Owner = this;

	AFGWeapon* NewWeapon = GetWorld()->SpawnActor<AFGWeapon>(NewWeaponSchematic->WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);

	if (NewWeapon)
	{
		CurrentWeapon = NewWeapon;
		CurrentWeapon->AttachToComponent(WeaponAttachPoint, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
		WeaponSchematic = NewWeaponSchematic;
	}
}

void AFGTurret::Fire()
{
	if (!CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->Fire();
}

void AFGTurret::Die()
{
	BP_OnDie();
}

float AFGTurret::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AFGTurret::StartDetection()
{
	StopDetection();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(TimerHandle, this, &AFGTurret::StartAsyncOverlap, DetectionInterval, false);
		bDetecting = true;
	}
}

void AFGTurret::StopDetection()
{
	bDetecting = false;

	if (TimerHandle.IsValid())
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(TimerHandle);
		}
	}
}

void AFGTurret::StartAsyncOverlap()
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(SphereOverlapActors), false);
	CollisionQueryParams.AddIgnoredActor(this);
	if (CurrentWeapon)
	{
		CollisionQueryParams.AddIgnoredActor(CurrentWeapon);
	}

	World->AsyncOverlapByObjectType(GetActorLocation(), FQuat::Identity, CachedCollisionObjectParams, CachedCollisionShape, CollisionQueryParams, &OverlapDelegate);
}

void AFGTurret::HandleAsyncOverlap(const FTraceHandle& TraceHandle, FOverlapDatum& OverlapDatum)
{
	UWorld* World = GetWorld();

	if (!World || !bDetecting)
	{
		return;
	}

	const FVector StartLocation = GetActorLocation();
	FHitResult Hit;
	FCollisionQueryParams LineTraceCollisionQueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(LineTraceSingle), false);
	LineTraceCollisionQueryParams.AddIgnoredActor(this);
	if (CurrentWeapon)
	{
		LineTraceCollisionQueryParams.AddIgnoredActor(CurrentWeapon);
	}

	TArray<AActor*> VisibleTargets;

	for (FOverlapResult& Overlap : OverlapDatum.OutOverlaps)
	{
		if (!Overlap.Actor.IsValid())
		{
			continue;
		}

		const bool bHit = World->LineTraceSingleByChannel(Hit, StartLocation, Overlap.Actor->GetActorLocation(), ECC_Visibility, LineTraceCollisionQueryParams);

		if (!bHit || Hit.Actor.Get() == Overlap.GetActor())
		{
			VisibleTargets.Add(Overlap.GetActor());
		}
	}

	for (AActor* AvailableTarget : VisibleTargets)
	{
		if (!AvailableTargets.Contains(AvailableTarget))
		{
			AvailableTargets.Add(AvailableTarget);
		}
	}

	for (int32 i = AvailableTargets.Num() - 1; i >= 0; --i)
	{
		AActor* AvailableTarget = AvailableTargets[i];
		if (!VisibleTargets.Contains(AvailableTarget))
		{
			AvailableTargets.RemoveAt(i);
			if (AvailableTarget == TargetActor)
			{
				TargetActor = nullptr;
			}
		}
	}

	if (!TargetActor && AvailableTargets.Num() > 0)
	{
		TargetActor = AvailableTargets[0];
	}

	StartDetection();
}

