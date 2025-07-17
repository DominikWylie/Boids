// Fill out your copyright notice in the Description page of Project Settings.

#include "Octant.h"

#include "DrawDebugHelpers.h"

#include "OctreeInterface.h"

Octant::Octant(
	const FVector& FCorner, 
	const FVector& SCorner, 
	const FVector& WorldLoc, 
	const TArray<IOctreeInterface*>& NList, 
	const uint16& MaxNodesPerOct,
	UWorld* Wrld)
	:
	FirstCorner(FCorner),
	SecondCorner(SCorner),
	WorldLocation(WorldLoc),
	NodeList(NList),
	MaxNodesPerOctant(MaxNodesPerOct),
	World(Wrld)
{

	DrawDebugBox(World, ((SecondCorner + WorldLocation) + (FirstCorner + WorldLocation)) / 2, (FirstCorner - SecondCorner) / 2, FColor::Red);

	//if exeeds max nodes subdivide and pass nodes to them to do thier thing

	if (NodeList.Num() > MaxNodesPerOctant) {
		//SubdivideTree();
	}

}

Octant::~Octant()
{
}

void Octant::Tick(/*UWorld* World*/)
{
	//DrawDebugBox(World, ((SecondCorner + WorldLocation) + (FirstCorner + WorldLocation)) / 2, (FirstCorner - SecondCorner) / 2, FColor::Red);

	//TArray<IOctreeInterface*> NodesToRemove;

	//for (IOctreeInterface*& Node : NodeList) {

	//	//check if all nodes are inside
	//	if (!IsWithinArea(Node->GetPosition())) {
	//		//kill node for now

	//		NodesToRemove.Add(Node);
	//	}
	//}

	//for (IOctreeInterface*& Node : NodesToRemove) {
	//	NodeList.Remove(Node);
	//	Node->Kill();
	//}

	////if exeeds max nodes subdivide and pass nodes to them to do thier thing

	//if (NodeList.Num() > MaxNodesPerOctant) {
	//	SubdivideTree();
	//}

	////if (subdevided) {
	////	for (TUniquePtr<Octant>& octant : Octants) {
	////		octant->Tick(World);
	////	}
	////}
}

void Octant::SubdivideTree()
{
	//if (subdevided) {
	//	return;
	//}


	FVector HalfDistance = (SecondCorner - FirstCorner) / 2;

	//top front left
	Octants[0] = MakeUnique<Octant>(
		FirstCorner,
		FirstCorner + HalfDistance,
		WorldLocation,
		NodeList,
		MaxNodesPerOctant,
		World
	);

	//top front right
	FVector Octant2FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y, FirstCorner.Z);
	Octants[1] = MakeUnique<Octant>(
		Octant2FirstCorner,
		Octant2FirstCorner + HalfDistance,
		WorldLocation,
		NodeList,
		MaxNodesPerOctant,
		World
	);

	//top back left
	FVector Octant3FirstCorner = FVector(FirstCorner.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z);
	Octants[2] = MakeUnique<Octant>(
		Octant3FirstCorner,
		Octant3FirstCorner + HalfDistance,
		WorldLocation,
		NodeList,
		MaxNodesPerOctant,
		World
	);

	//top back right
	FVector Octant4FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z);
	Octants[3] = MakeUnique<Octant>(
		Octant4FirstCorner,
		Octant4FirstCorner + HalfDistance,
		WorldLocation,
		NodeList,
		MaxNodesPerOctant,
		World
	);

	//bottom front left
	FVector Octant5FirstCorner = FVector(FirstCorner.X, FirstCorner.Y, FirstCorner.Z + HalfDistance.Z);
	Octants[4] = MakeUnique<Octant>(
		Octant5FirstCorner,
		Octant5FirstCorner + HalfDistance,
		WorldLocation,
		NodeList,
		MaxNodesPerOctant,
		World
	);

	//bottom front right
	FVector Octant6FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y, FirstCorner.Z + HalfDistance.Z);
	Octants[5] = MakeUnique<Octant>(
		Octant6FirstCorner,
		Octant6FirstCorner + HalfDistance,
		WorldLocation,
		NodeList,
		MaxNodesPerOctant,
		World
	);

	//bottom back left
	FVector Octant7FirstCorner = FVector(FirstCorner.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z + HalfDistance.Z);
	Octants[6] = MakeUnique<Octant>(
		Octant7FirstCorner,
		Octant7FirstCorner + HalfDistance,
		WorldLocation,
		NodeList,
		MaxNodesPerOctant,
		World
	);

	//bottom back right
	FVector Octant8FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z + HalfDistance.Z);
	Octants[7] = MakeUnique<Octant>(
		Octant8FirstCorner,
		Octant8FirstCorner + HalfDistance,
		WorldLocation,
		NodeList,
		MaxNodesPerOctant,
		World
	);

	//FVector HalfDistance = (SecondCorner - FirstCorner) / 2;

	////FVector makeItClearthe4ththisbetemp = FVector(20.f, 20.f, 20.f);

	////top front left
	//Octants[0] = MakeUnique<Octant>(FirstCorner, FirstCorner + HalfDistance);

	////top front right
	//FVector Octant2FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y, FirstCorner.Z);
	//Octants[1] = MakeUnique<Octant>(Octant2FirstCorner, Octant2FirstCorner + HalfDistance);

	////top back left
	//FVector Octant3FirstCorner = FVector(FirstCorner.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z);
	//Octants[2] = MakeUnique<Octant>(Octant3FirstCorner, Octant3FirstCorner + HalfDistance);

	////top back right
	//FVector Octant4FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z);
	//Octants[3] = MakeUnique<Octant>(Octant4FirstCorner, Octant4FirstCorner + HalfDistance);



	////bottom front left
	//FVector Octant5FirstCorner = FVector(FirstCorner.X, FirstCorner.Y, FirstCorner.Z + HalfDistance.Z);
	//Octants[4] = MakeUnique<Octant>(Octant5FirstCorner, Octant5FirstCorner + HalfDistance);

	////bottom front right
	//FVector Octant6FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y, FirstCorner.Z + HalfDistance.Z);
	//Octants[5] = MakeUnique<Octant>(Octant6FirstCorner, Octant6FirstCorner + HalfDistance);

	////bottom back left
	//FVector Octant7FirstCorner = FVector(FirstCorner.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z + HalfDistance.Z);
	//Octants[6] = MakeUnique<Octant>(Octant7FirstCorner, Octant7FirstCorner + HalfDistance);

	////bottom back right
	//FVector Octant8FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z + HalfDistance.Z);
	//Octants[7] = MakeUnique<Octant>(Octant8FirstCorner, Octant8FirstCorner + HalfDistance);

	//subdevided = true;
}

bool Octant::IsWithinArea(const FVector& Location)
{
	FVector FirstWorldCorner = FirstCorner + WorldLocation;
	FVector SecondWorldCorner = SecondCorner + WorldLocation;

	if (Location.X > FirstWorldCorner.X ||
		Location.Y > FirstWorldCorner.Y ||
		Location.Z > FirstWorldCorner.Z) {
		return false;
	}

	if (Location.X < SecondWorldCorner.X ||
		Location.Y < SecondWorldCorner.Y ||
		Location.Z < SecondWorldCorner.Z) {
		return false;
	}

	return true;
}
