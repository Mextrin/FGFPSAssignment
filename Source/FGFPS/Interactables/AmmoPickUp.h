#pragma once

#include "Player/FGPlayerCharacter.h"
#include "AmmoPickUp.generated.h"


UCLASS()
class FGFPS_API AFGAmmoPickUp : public AActor
{
	GENERATED_BODY()
public:

	AFGAmmoPickUp(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* HitBoxRange = nullptr;

	UPROPERTY()
	float RespawnTimer = 0.0f;
	bool Triggered = false;

	UFUNCTION()
	void PickUp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:

	void DisablePickUp();

	void EnablePickUp();

};
