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

	//DrawDebugSphere(GetWorld(), GetActorLocation(), ProtectedRange, 10, FColor::Red);
	//DrawDebugSphere(GetWorld(), GetActorLocation(), VisualRange, 10, FColor::Green);
}

FVector ABoid::GetPosition() const
{
	return GetActorLocation();
}

FVector ABoid::GetForwardVector() const
{
	return GetActorForwardVector();
}

float ABoid::GetSpeed() const
{
	return Speed;
}

void ABoid::Kill()
{
	Destroy();
}

void ABoid::CalculateTrajectory(TArray<IOctreeInterface*> Boids, float dt)
{
	FRotator ActorRotation = GetActorRotation();
	FVector ActorLocation = GetActorLocation();
	FQuat CurrentActorQuat = GetActorQuat();

	if (ActorLocation.X > UpperBounds.X ||
		ActorLocation.Y > UpperBounds.Y ||
		ActorLocation.Z > UpperBounds.Z ||
		ActorLocation.X < LowerBounds.X ||
		ActorLocation.Y < LowerBounds.Y ||
		ActorLocation.Z < LowerBounds.Z) 
	{
		FVector BoundsTargetDirection = CentreOfBounds - GetActorLocation();
		BoundsTargetDirection.Normalize();

		FQuat BoundsTargetQuat = BoundsTargetDirection.Rotation().Quaternion();

		FQuat BoundsNewDirection = FQuat::Slerp(CurrentActorQuat, BoundsTargetQuat, BoundsTurningSpeed * dt);

		BoundsNewDirection.Normalize();

		SetActorRotation(BoundsNewDirection);

		//this overrides the ai if it hits the edge
		return;
	}



	FVector TargetDirection = FVector::ZeroVector;

	double VisualRangeSquared = FMath::Square(VisualRange);
	double ProtectedRangeSquared = FMath::Square(ProtectedRange);

	FVector PositionAverage = FVector::ZeroVector;
	FVector ForwardAverage = FVector::ZeroVector;
	FVector CloseBoidPositionAverage = FVector::ZeroVector;
	float SpeedAverage = 0;
	uint16 NeighboringBoids = 0;

	for (IOctreeInterface*& boid : Boids) {
		FVector boidPosition = boid->GetPosition();
		double BoidDistanceSquared = FVector::DistSquared(boidPosition, ActorLocation);

		//only care about within visual range and not protected range
		if (BoidDistanceSquared < VisualRangeSquared && BoidDistanceSquared > ProtectedRangeSquared) {

			//cohesion
			PositionAverage += boidPosition;
			
			//alignment - direction and speed
			ForwardAverage += boid->GetForwardVector();
			SpeedAverage += boid->GetSpeed();
			
			NeighboringBoids++;
		}
		else if (BoidDistanceSquared < ProtectedRangeSquared) {
			//seperation
			CloseBoidPositionAverage += ActorLocation - boidPosition;
		}


	}

	if (NeighboringBoids > 0) {
		//expensive
		PositionAverage = ((PositionAverage / NeighboringBoids) - ActorLocation) * CenteringFactor;
		ForwardAverage = ForwardAverage.GetSafeNormal() * MatchingFactor;
		SpeedAverage /= NeighboringBoids;

		CloseBoidPositionAverage *= AvoidBoidsFactor;

		TargetDirection = (PositionAverage + ForwardAverage + CloseBoidPositionAverage) * .33333f;

		//i think its always going up is cos forard average is normalised and position average is not
		//TargetDirection = (PositionAverage + ForwardAverage) * .5f;
		//TargetDirection = PositionAverage;
		//TargetDirection = ForwardAverage;


		//maybe change speed depending on if target direction is further away than a caertaiun area 
		// but would mess with the boids on the edges

		//if ((Speed - SpeedAverage) > SpeedTolerance) {
		//	Speed += Acceleration;
		//}
		//else if ((Speed - SpeedAverage) < -SpeedTolerance) {
		//	Speed -= Acceleration;
		//}

		//TargetDirection = PositionAverage;

		//move
		TargetDirection.Normalize();

		FQuat TargetQuat = TargetDirection.Rotation().Quaternion();

		FQuat NewDirection = FQuat::Slerp(CurrentActorQuat, TargetQuat, GeneralTurningSpeed * dt);
		//FQuat NewDirection = FQuat::FastLerp(CurrentQuat, TargetQuat, GeneralTurningSpeed * dt);

		SetActorRotation(NewDirection);
	}

}
