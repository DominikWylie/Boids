// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "OctreeInterface.h"

#include "Boid.generated.h"

class AOctreeMain;

UCLASS()
class BOIDS_API ABoid : public AActor, public IOctreeInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();

	void initialise(
		const TObjectPtr<AOctreeMain>& Octree, 
		const FVector& FirstBounds, 
		const FVector& Secondbounds,
		const FVector& CentreBounds);

	UPROPERTY(EditAnywhere)
	float Speed = 1300.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetPosition() const override;

	void Kill() override;

	bool move = true;

	int32 NodeMatchIDTemp = 0;

private:
	
	float 
		VisualRange = 400.f, 
		ProtectedRange = 200.f,
		BoundsTurningSpeed = 3.f,
		GeneralTurningSpeed = 3.f;

	FVector
		UpperBounds = FVector::ZeroVector,
		LowerBounds = FVector::ZeroVector,
		CentreOfBounds = FVector::ZeroVector;

	TObjectPtr<AOctreeMain> Octree;

	void CalculateTrajectory(TArray<IOctreeInterface*> Boids, float dt);
};
