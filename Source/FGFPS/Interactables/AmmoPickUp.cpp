#include "AmmoPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Player/FGPlayerCharacter.h"
#include "Engine/Engine.h"
#include "Components/SceneComponent.h"

AFGAmmoPickUp::AFGAmmoPickUp(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitBoxRange = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("HitBoxRange"));
	HitBoxRange->SetupAttachment(Mesh);
	HitBoxRange->RelativeLocation = FVector(0.f, 0.f, 0.f);
	HitBoxRange->SetSphereRadius(32.0f);

}

void AFGAmmoPickUp::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AFGAmmoPickUp::PickUp);
}

void AFGAmmoPickUp::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Mesh->AddLocalRotation(FRotator(0, 1.f, 0));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"),RespawnTimer));
	RespawnTimer -= DeltaSeconds;
	if (RespawnTimer <= 0)
	{
		EnablePickUp();
		Triggered = false;
	}
}

void AFGAmmoPickUp::PickUp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AFGPlayerCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("You pickup")));
		Cast<AFGPlayerCharacter>(OtherActor)->AddAmmo();
		DisablePickUp();
	}
}

void AFGAmmoPickUp::DisablePickUp()
{
	Mesh->ToggleVisibility(false);
	HitBoxRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RespawnTimer = 10.0f;
	Triggered = true;
}

void AFGAmmoPickUp::EnablePickUp()
{
	if (Triggered)
	{
		Mesh->ToggleVisibility(true);
		HitBoxRange->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else{}
}

