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

void ABoid::initialise(
	const TObjectPtr<AOctreeMain>& Otree,
	const FVector& FBounds,
	const FVector& Sbounds,
	const FVector& CBounds,
	ImGuiModifier* IGMods)
{
	Octree = Otree;
	UpperBounds = FBounds;
	LowerBounds = Sbounds;
	CentreOfBounds = CBounds;

	ImGuiMods = IGMods;

	check(ImGuiMods);
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

	CalculateTrajectory(Octree->NodeQuery(GetActorLocation(), ImGuiMods->VisualRange), DeltaTime);
	
	if (move) {
		//rotate then translate
		SetActorLocation(GetActorLocation() + (GetActorForwardVector() * (ImGuiMods->Speed + SpeedDifference) * DeltaTime));
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
	return ImGuiMods->Speed + SpeedDifference;
}

void ABoid::Kill()
{
	Destroy();
}

void ABoid::CalculateTrajectory(TArray<IOctreeInterface*> Boids, float dt)
{
	const FRotator ActorRotation = GetActorRotation();
	const FVector ActorLocation = GetActorLocation();
	const FQuat CurrentActorQuat = GetActorQuat();

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

		BoundsTargetQuat.X = 0;

		FQuat BoundsNewDirection = FQuat::FastLerp(CurrentActorQuat, BoundsTargetQuat, BoundsTurningSpeed * dt);

		BoundsNewDirection.X = 0;
		BoundsNewDirection.Normalize();

		SetActorRotation(BoundsNewDirection);

		//this overrides the ai if it hits the edge
		return;
	}



	FVector TargetDirection = FVector::ZeroVector;

	double VisualRangeSquared = FMath::Square(ImGuiMods->VisualRange);
	double ProtectedRangeSquared = FMath::Square(ImGuiMods->ProtectedRange);

	FVector PositionAverage = FVector::ZeroVector;
	FVector ForwardAverage = FVector::ZeroVector;
	FVector CloseBoidPositionAverage = FVector::ZeroVector;
	float SpeedAverage = 0;
	uint16 NeighboringBoids = 0;

	for (IOctreeInterface*& boid : Boids) {
		if (boid == this) continue;

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

		PositionAverage = ((PositionAverage / NeighboringBoids) - ActorLocation) * ImGuiMods->CenteringFactor;
		//SpeedAverage /= NeighboringBoids;

		TargetDirection = ((PositionAverage * ImGuiMods->CenteringFactor) + (ForwardAverage * ImGuiMods->MatchingFactor) + (CloseBoidPositionAverage * ImGuiMods->AvoidBoidsFactor)) * .33333f;
		
		if (FVector::DistSquared(ActorLocation, TargetDirection) < ImGuiMods->ProtectedRange) {
			if (FVector::DotProduct(GetActorForwardVector(), TargetDirection) > 0.01) {
				SpeedDifference -= Acceleration;
			}
			else {
				SpeedDifference += Acceleration;
			}
		}
		else {
			if (FVector::DotProduct(GetActorForwardVector(), TargetDirection) > 0.01) {
				SpeedDifference += Acceleration;
			}
			else {
				SpeedDifference -= Acceleration;
			}
		}

		if (ImGuiMods->Speed + SpeedDifference > ImGuiMods->Speed + (ImGuiMods->Speed * MaxMinSpeedPercent)) {
			SpeedDifference = (ImGuiMods->Speed * MaxMinSpeedPercent) - 0.001f;
		}
		//speeddifference will be negative if reducing speed
		else if (ImGuiMods->Speed + SpeedDifference < ImGuiMods->Speed - (ImGuiMods->Speed * MaxMinSpeedPercent)) {
			SpeedDifference = (ImGuiMods->Speed * MaxMinSpeedPercent) + 0.001f;
		}

		//if(TargetDirection - ActorLocation)
		
		TargetDirection = TargetDirection.GetSafeNormal();

		//if ((Speed - SpeedAverage) > SpeedTolerance) {
		//	Speed += Acceleration;
		//}
		//else if ((Speed - SpeedAverage) < -SpeedTolerance) {
		//	Speed -= Acceleration;
		//}

		//TargetDirection = PositionAverage;

		FQuat TargetQuat = TargetDirection.Rotation().Quaternion();

		float AngleDiff = CurrentActorQuat.AngularDistance(TargetQuat) * ImGuiMods->GeneralTurningSpeed;

		FQuat NewDirection = FQuat::FastLerp(CurrentActorQuat, TargetQuat, AngleDiff * dt);

		SetActorRotation(NewDirection);
	}

}
