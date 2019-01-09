#pragma once

#include "Character/FGCharacter.h"
#include "FGEnemyCharacter.generated.h"

UCLASS()
class FGFPS_API AFGEnemyCharacter : public AFGCharacter
{
	GENERATED_BODY()
public:

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Kill this character
	UFUNCTION(BlueprintCallable, Category = "FG|EnemyCharacter")
	void Die();

	void RemoveBody();

	// The enemy character will attempt to move towards this location
	UFUNCTION(BlueprintCallable, Category = "FG|EnemyCharacter")
	void SetTargetLocation(const FVector& TargetLocation);

protected:
	virtual void BeginPlay() override;

	static const float TimeUntilRagdoll;

	UPROPERTY(EditAnywhere, Category = Animation)
	class UAnimMontage* StaggerMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = Animation)
	TMap<FName, UAnimMontage*> HitReactions;

	UFUNCTION()
	void DoRagdoll();

	FTimerHandle RagdollTimerHandle;

	UPROPERTY(EditAnywhere, Category = Animation)
	class UAnimMontage* DeathMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	float Health = 100.0f;

	bool bDead = false;
	bool bRagdoll = false;
};
