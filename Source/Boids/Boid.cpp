// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "OctreeMain.h"


// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NodeMatchIDTemp = FMath::RandRange(1, 1000);

}

void ABoid::initialise(TObjectPtr<AOctreeMain> Otree)
{
	Octree = Otree;
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

	//CalculateTrajectory(Octree->NodeQuery(GetActorLocation(), VisualRange));
	
	if (move) {
		SetActorLocation(GetActorLocation() + (GetActorUpVector() * Speed * DeltaTime));
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

void ABoid::TempPause()
{
	move = false;
}

void ABoid::colourin()
{
	float radius = 100;
	FVector location = FVector(0, 0, 1200);
	DrawDebugSphere(GetWorld(), GetActorLocation(), radius, 15, FColor::Purple, false);
}

int32 ABoid::id()
{
	return NodeMatchIDTemp;
}

void ABoid::CalculateTrajectory(TArray<IOctreeInterface*> Boids)
{
	FVector Displacement;

	for (IOctreeInterface*& Boid : Boids) {

		Displacement = GetActorLocation() - Boid->GetPosition();

		//FMath::Abs
		if (FMath::Abs(Displacement.Size()) < VisualRange) {

		}
	}

	//for (int i = 0; i < Boids.Num(); i++) {

	//	for (int j = 0; j < Boids.Num(); j++) {
	//		if (i == j) continue;

	//		Displacement = GetActorLocation - Boids[]
	//	}
	//}
}
