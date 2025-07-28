// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "OctreeMain.h"
#include "BoidSpawner.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NodeMatchIDTemp = FMath::RandRange(1, 1000);

}

void ABoid::initialise(const TObjectPtr<AOctreeMain>& Otree, const FVector& FBounds, const FVector& Sbounds, const FVector& CBounds)
{
	Octree = Otree;
	UpperBounds = FBounds;
	LowerBounds = Sbounds;
	CentreOfBounds = CBounds;
}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalculateTrajectory(Octree->NodeQuery(GetActorLocation(), VisualRange), DeltaTime);
	
	if (move) {
		//rotate then translate
		SetActorLocation(GetActorLocation() + (GetActorForwardVector() * Speed * DeltaTime));
	}
}

FVector ABoid::GetPosition() const
{
	return GetActorLocation();
}

void ABoid::Kill()
{
	Destroy();
}

void ABoid::CalculateTrajectory(TArray<IOctreeInterface*> Boids, float dt)
{
	FRotator Rotation = GetActorRotation();
	FVector Location = GetActorLocation();
	FQuat CurrentQuat = GetActorQuat();

	if (Location.X > UpperBounds.X ||
		Location.Y > UpperBounds.Y ||
		Location.Z > UpperBounds.Z ||
		Location.X < LowerBounds.X ||
		Location.Y < LowerBounds.Y ||
		Location.Z < LowerBounds.Z) 
	{
		FVector BoundsTargetDirection = CentreOfBounds - GetActorLocation();
		BoundsTargetDirection.Normalize();

		FQuat BoundsTargetQuat = BoundsTargetDirection.Rotation().Quaternion();

		FQuat BoundsNewDirection = FQuat::Slerp(CurrentQuat, BoundsTargetQuat, BoundsTurningSpeed * dt);

		BoundsNewDirection.Normalize();

		SetActorRotation(BoundsNewDirection);

		//this overrides the ai if it hits the edge
		return;
	}

	FVector TargetDirection = FVector::ZeroVector;

	double VisualRangeSquared = FMath::Square(VisualRange);
	double ProtectedRangeSquared = FMath::Square(ProtectedRange);

	FVector PositionAverage = FVector::ZeroVector;
	uint16 NeighboringBoids = 0;

	//cohesion
	for (IOctreeInterface*& boid : Boids) {
		FVector boidPosition = boid->GetPosition();
		double BoidDistanceSquared = FVector::DistSquared(boidPosition, Location);

		//only care about within visual range and not protected range
		if (BoidDistanceSquared < VisualRangeSquared && BoidDistanceSquared > ProtectedRangeSquared) {

			PositionAverage += boidPosition;
			NeighboringBoids++;
		}
	}

	if (NeighboringBoids > 0) {
		PositionAverage /= NeighboringBoids;

		TargetDirection += PositionAverage;

		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("more than  zero bods!"));
	}

	//move
	TargetDirection.Normalize();

	FQuat TargetQuat = TargetDirection.Rotation().Quaternion();

	FQuat NewDirection = FQuat::Slerp(CurrentQuat, TargetQuat, GeneralTurningSpeed * dt);

	SetActorRotation(NewDirection);

	//FVector DisplacementToOtherBoid;

	//FVector
	//	CloseRangeDisplacement = FVector::ZeroVector,
	//	re = FVector::ZeroVector,
	//	ro = FVector::ZeroVector;

	//float NeighboringBoids = 0;

	//for (IOctreeInterface*& Boid : Boids) {

	//	DisplacementToOtherBoid = GetActorLocation() - Boid->GetPosition();


	//	if (DisplacementToOtherBoid.SizeSquared() < FMath::Square(VisualRange)) {

	//		float DisplacementToOtherBoidSquared = DisplacementToOtherBoid.SizeSquared();

	//		if (DisplacementToOtherBoidSquared < FMath::Square(ProtectedRange)) {
	//			CloseRangeDisplacement += DisplacementToOtherBoid;
	//		}
	//		else {
	//			//in visual but not protected

	//		}
	//	}
	//}

	////for (int i = 0; i < Boids.Num(); i++) {

	////	for (int j = 0; j < Boids.Num(); j++) {
	////		if (i == j) continue;

	////		Displacement = GetActorLocation - Boids[]
	////	}
	////}
}
