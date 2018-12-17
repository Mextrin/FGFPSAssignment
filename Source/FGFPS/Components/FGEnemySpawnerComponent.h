#pragma once

UCLASS
class FGFPS_API AFGEnemySpawnerComponent : public AActor
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = Spawner)
	FVector Spawnlocation;

	UFUNCTION()
	void SpawnEnemy();

};