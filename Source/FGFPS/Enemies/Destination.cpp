// Fill out your copyright notice in the Description page of Project Settings.

#include "Destination.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "FGEnemyCharacter.h"

// Sets default values
ADestination::ADestination()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DestRange = CreateDefaultSubobject<USphereComponent>("Range sphere");
	RootComponent = DestRange;

}

// Called when the game starts or when spawned
void ADestination::BeginPlay()
{
	Super::BeginPlay();

	DestRange->OnComponentBeginOverlap.AddDynamic(this, &ADestination::OnEnterDest);
}

// Called every frame
void ADestination::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADestination::OnEnterDest(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AFGEnemyCharacter::StaticClass()))
	{
		OtherActor->Destroy();
	}
}


