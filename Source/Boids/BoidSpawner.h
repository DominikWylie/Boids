// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "OctreeMain.h"

#include "BoidSpawner.generated.h"

class ABoid;
class AOctreeMain;

UCLASS()
class BOIDS_API ABoidSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoidSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnTimer();

	FTimerHandle MyTimerHandle;
	UWorld* World;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ABoid> BoidBlueprint;

	TObjectPtr<AOctreeMain> Octree;
};
