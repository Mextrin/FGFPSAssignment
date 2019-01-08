#pragma once

#include "GameFramework/Actor.h"
#include "Interfaces/FGWeaponInterface.h"
#include "CollisionQueryParams.h"
#include "WorldCollision.h"
#include "FGTurret.generated.h"

UCLASS()
class FGFPS_API AFGTurret : public AActor, public IFGWeaponInterface
{
	GENERATED_BODY()
public:
	AFGTurret(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	virtual void Tick(float DeltaSeconds) override;

	bool GetShootDirection_Implementation(FVector& StartLocation, FVector& ForwardDirection) const;

	UFUNCTION(BlueprintCallable, Category = "FG|Turret")
	void CreateWeapon(class UFGWeaponSchematic* NewWeaponSchematic);

	void Fire();

	UFUNCTION(BlueprintCallable)
	void Reload();

	void StartDetection();
	void StopDetection();

protected:

	UFUNCTION()
	void StartAsyncOverlap();

	void HandleAsyncOverlap(const FTraceHandle& TraceHandle, FOverlapDatum& OverlapDatum);

	FOverlapDelegate OverlapDelegate;

	FCollisionShape CachedCollisionShape;

	FTimerHandle TimerHandle;

	UPROPERTY(Transient)
	TArray<AActor*> AvailableTargets;

	UPROPERTY(EditAnywhere, Category = Collision)
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;

	FCollisionObjectQueryParams CachedCollisionObjectParams;

	FCollisionQueryParams CachedCollisionQueryParams;

	UPROPERTY(EditAnywhere, Category = Detection)
	float DetectionInterval = 0.5f;

	UPROPERTY(EditAnywhere, Category = Detection)
	float VisibilityRange = 800.0f;

	UPROPERTY(Transient)
	AActor* TargetActor = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly)
	class AFGWeapon* CurrentWeapon = nullptr;

	UPROPERTY(Transient)
	class UFGWeaponSchematic* WeaponSchematic = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = Lua)
	class UFGLuaComponent* LuaComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = Interactable)
	class UFGInteractableComponent* Interactable = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = Scene)
	class USceneComponent* WeaponAttachPoint = nullptr;

	bool bDetecting = false;
};
