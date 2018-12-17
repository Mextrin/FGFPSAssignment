#pragma once

#include "GameFramework/Character.h"
#include "FGEnemyCharacter.generated.h"

UCLASS()
class FGFPS_API AFGEnemyCharacter : public ACharacter
{
	GENERATED_BODY()
public:

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Die();

protected:
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
