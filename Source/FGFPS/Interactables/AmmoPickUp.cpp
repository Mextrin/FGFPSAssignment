#include "AmmoPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Player/FGPlayerCharacter.h"
#include "Engine/Engine.h"
#include "Components/SceneComponent.h"

AFGAmmoPickUp::AFGAmmoPickUp(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitBoxRange = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("HitBoxRange"));
	HitBoxRange->SetupAttachment(Mesh);
	HitBoxRange->SetSphereRadius(32.0f);
	HitBoxRange->SetGenerateOverlapEvents(true);

}

void AFGAmmoPickUp::BeginPlay()
{
	Super::BeginPlay();
	//HitBoxRange->OnComponentBeginOverlap.AddDynamic(this, &AFGAmmoPickUp::PickUp);
}

void AFGAmmoPickUp::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Mesh->AddLocalRotation(FRotator(0, 1.f, 0));
	RespawnTimer -= DeltaSeconds;
	if (RespawnTimer <= 0)
	{
		EnablePickUp();
	}
}

void AFGAmmoPickUp::PickUp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You pickup")));
	if (OtherActor->IsA(AFGPlayerCharacter::StaticClass()))
	{
		//AFGPlayerCharacter::CurrentWeapon->CurrtAmmo += AmmoAmount;
		DisablePickUp();
	}
}

void AFGAmmoPickUp::DisablePickUp()
{
	Mesh->Deactivate();
	HitBoxRange->Deactivate();
	RespawnTimer = 10.0f;
}

void AFGAmmoPickUp::EnablePickUp()
{
	Mesh->Activate(true);
	HitBoxRange->Activate(true);
}

