#include "FGEnemyCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

const float AFGEnemyCharacter::TimeUntilRagdoll = 0.25f;

float AFGEnemyCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bDead)
	{
		return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent& PointDamageEvent = *static_cast<const FPointDamageEvent*>(&DamageEvent);

		FHitResult Hit;
		const FVector StartLocation = PointDamageEvent.HitInfo.ImpactPoint;
		const FVector EndLocation = PointDamageEvent.HitInfo.ImpactPoint + (PointDamageEvent.ShotDirection * 1000.0f);
	
		if (UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), StartLocation, EndLocation,
			FName(TEXT("CharacterBody")), false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, Hit, true))
		{
			if (Hit.GetActor() != this)
			{
				Damage = 0.0f;
			}
			else
			{
				if (UAnimMontage* HitMontage = HitReactions.FindRef(Hit.BoneName))
				{
					PlayAnimMontage(HitMontage);
				}
				else
				{
					PlayAnimMontage(StaggerMontage);
				}
				
				UKismetSystemLibrary::PrintString(GetWorld(), Hit.BoneName.ToString());
			}
		}
		else
		{
			Damage = 0.0f;
		}
	}


	if (Damage > SMALL_NUMBER)
	{
		Health -= Damage;

		if (Health <= 0.0f)
		{
			Die();
		}
	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void AFGEnemyCharacter::Die()
{
	if (bDead)
	{
		return;
	}

	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}

	bDead = true;
	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.ClearTimer(RagdollTimerHandle);
	TimerManager.SetTimer(RagdollTimerHandle, this, &AFGEnemyCharacter::DoRagdoll, TimeUntilRagdoll, false);
}

void AFGEnemyCharacter::DoRagdoll()
{
	if (!bDead || bRagdoll)
	{
		return;
	}

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName(TEXT("Ragdoll")));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetAnimInstanceClass(nullptr);
	bRagdoll = true;
}

