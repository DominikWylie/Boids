// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "BoidSpawner.generated.h"

class ABoid;
class AOctreeMain;

UCLASS()
class BOIDS_API ABoidSpawner : public AActor
{
	GENERATED_BODY()

public:
	ABoidSpawner();

	virtual void Tick(float DeltaTime) override;

	//less readable && more fast
	TPair<FVector, FVector> GetBounds() { return TPair<FVector, FVector>(firstBounds, SecondBounds); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ABoid> BoidBlueprint;

protected:
	virtual void BeginPlay() override;

	void Spawn();
private:

	FTimerHandle MyTimerHandle;
	UWorld* World;

	TObjectPtr<AOctreeMain> Octree;

	FVector 
		firstBounds = FVector::ZeroVector,
		SecondBounds = FVector::ZeroVector;

	float BoundsBuffer = 2000.f;

	int32 BoidCount = 0;
};
