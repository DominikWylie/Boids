// Fill out your copyright notice in the Description page of Project Settings.

#include "Octant.h"

#include "DrawDebugHelpers.h"

Octant::Octant(FVector FCorner, FVector SCorner) : 
	FirstCorner(FCorner), 
	SecondCorner(SCorner)
{
}

Octant::~Octant()
{
}

void Octant::Tick(UWorld* World, FVector WorldLocation)
{
	DrawDebugBox(World, ((SecondCorner + WorldLocation) + (FirstCorner + WorldLocation)) / 2, (FirstCorner - SecondCorner) / 2, FColor::Red);

	if (subdevided) {
		for (TUniquePtr<Octant>& octant : Octants) {
			octant->Tick(World, WorldLocation);
		}
	}
}

void Octant::Subdivide()
{
	if (subdevided) {
		return;
	}

	FVector HalfDistance = (SecondCorner - FirstCorner) / 2;

	//FVector makeItClearthe4ththisbetemp = FVector(20.f, 20.f, 20.f);

	//top front left
	Octants[0] = MakeUnique<Octant>(FirstCorner, FirstCorner + HalfDistance);

	//top front right
	FVector Octant2FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y, FirstCorner.Z);
	Octants[1] = MakeUnique<Octant>(Octant2FirstCorner, Octant2FirstCorner + HalfDistance);

	//top back left
	FVector Octant3FirstCorner = FVector(FirstCorner.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z);
	Octants[2] = MakeUnique<Octant>(Octant3FirstCorner, Octant3FirstCorner + HalfDistance);

	//top back right
	FVector Octant4FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z);
	Octants[3] = MakeUnique<Octant>(Octant4FirstCorner, Octant4FirstCorner + HalfDistance);



	//bottom front left
	FVector Octant5FirstCorner = FVector(FirstCorner.X, FirstCorner.Y, FirstCorner.Z + HalfDistance.Z);
	Octants[4] = MakeUnique<Octant>(Octant5FirstCorner, Octant5FirstCorner + HalfDistance);

	//bottom front right
	FVector Octant6FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y, FirstCorner.Z + HalfDistance.Z);
	Octants[5] = MakeUnique<Octant>(Octant6FirstCorner, Octant6FirstCorner + HalfDistance);

	//bottom back left
	FVector Octant7FirstCorner = FVector(FirstCorner.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z + HalfDistance.Z);
	Octants[6] = MakeUnique<Octant>(Octant7FirstCorner, Octant7FirstCorner + HalfDistance);

	//bottom back right
	FVector Octant8FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z + HalfDistance.Z);
	Octants[7] = MakeUnique<Octant>(Octant8FirstCorner, Octant8FirstCorner + HalfDistance);

	subdevided = true;
}
