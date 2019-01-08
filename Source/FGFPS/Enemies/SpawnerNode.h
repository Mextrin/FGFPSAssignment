// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerNode.generated.h"

UCLASS()
class FGFPS_API ASpawnerNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* SpawnRange;

	UFUNCTION()
	void OnEnemyExitRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	

	UFUNCTION()
	bool Spawn(TSubclassOf<class AFGEnemyCharacter> EnemyType);

	bool CanSpawn = true;
};
