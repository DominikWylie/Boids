// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImGuiModifier.h"

#include "BoidSpawner.generated.h"

class ABoid;
class AOctreeMain;

UCLASS()
class BOIDS_API ABoidSpawner : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bBoundingBoxVisibiliy = true;

	ABoidSpawner();

	virtual void Tick(float DeltaTime) override;

	//less readable && more fast
	TPair<FVector, FVector> GetBounds() { return TPair<FVector, FVector>(FirstBounds, SecondBounds); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ABoid> BoidBlueprint;



protected:
	virtual void BeginPlay() override;

	void Spawn();
private:

	FImGuiModifier ImGuiMods;

	std::shared_ptr<const FBoidPreset> boidPreset;

	void Imgui();

	FTimerHandle MyTimerHandle;
	//UWorld* World;

	TObjectPtr<AOctreeMain> Octree;

	FVector 
		FirstBounds = FVector::ZeroVector,
		SecondBounds = FVector::ZeroVector,
		CentreBounds = FVector::ZeroVector;

	APlayerController* PC;

	float BoundsBuffer = 100.f;

	bool ShowVisualRangeSpheres = false;
};
