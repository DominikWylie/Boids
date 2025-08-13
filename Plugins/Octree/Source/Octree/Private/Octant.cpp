// Fill out your copyright notice in the Description page of Project Settings.

#include "Octant.h"

#include "DrawDebugHelpers.h"

#include "OctreeInterface.h"

Octant::Octant(
	const FVector& FCorner, 
	const FVector& SCorner, 
	const FVector& WorldLoc, 
	const TArray<IOctreeInterface*>& NList, 
	const uint16 MaxNodesPerOct,
	UWorld* Wrld, 
	const bool bBBoxVisibiliy)
	:
	FirstCorner(FCorner),
	SecondCorner(SCorner),
	WorldLocation(WorldLoc),
	NodeList(NList),
	MaxNodesPerOctant(MaxNodesPerOct),
	World(Wrld),
	bBoundingBoxVisibiliy(bBBoxVisibiliy)
{
#if WITH_EDITOR
	//if (bBoundingBoxVisibiliy) {
	//	DrawDebugBox(World, ((SecondCorner + WorldLocation) + (FirstCorner + WorldLocation)) / 2, (FirstCorner - SecondCorner) / 2, BoxColour);
	//}
#endif

	//if exeeds max nodes subdivide and pass nodes to them to do thier thing

	//DrawDebugBox(World, ((SecondCorner + WorldLocation) + (FirstCorner + WorldLocation)) / 2, (FirstCorner - SecondCorner) / 2, BoxColour);

	if (NodeList.Num() > MaxNodesPerOctant) {
		SubdivideTree();
	}


}

Octant::~Octant()
{
}

TArray<IOctreeInterface*> Octant::NodeQuery(const FVector& Centre, const float Extent)
{
	//second is min, first is max

	int32 DistanceMin = 0;

	FVector FirstWorldCorner = FirstCorner + WorldLocation;
	FVector SecondWorldCorner = SecondCorner + WorldLocation;

	for (int32 i = 0; i < 3; i++) {
		if (Centre[i] < SecondWorldCorner[i]) DistanceMin += FMath::Square(Centre[i] - SecondWorldCorner[i]);
		else if (Centre[i] > FirstWorldCorner[i]) DistanceMin += FMath::Square(Centre[i] - FirstWorldCorner[i]);
	}

	if (!(DistanceMin <= FMath::Square(Extent))) {
		//if out of bounds send back an empty array
		return TArray<IOctreeInterface*>();
	}

	if (!subdevided) {
		//end of tree
		if (bBoundingBoxVisibiliy) {
			DrawDebugBox(World, ((SecondCorner + WorldLocation) + (FirstCorner + WorldLocation)) / 2, (FirstCorner - SecondCorner) / 2, BoxColour);
			//DrawDebugString(World, FirstCorner + WorldLocation, FString::FromInt(NodeMatchIDTemp));
		}
		return NodeList;
	}

	TArray<IOctreeInterface*> QuerydNodeList;

	for (const TUniquePtr<Octant>& octant : Octants) {
		QuerydNodeList += octant->NodeQuery(Centre, Extent);
	}

	return QuerydNodeList;
}

void Octant::SubdivideTree()
{
	FVector HalfDistance = (SecondCorner - FirstCorner) / 2;

	//top front left
	TArray<IOctreeInterface*> TopFrontLeftNodes;

	SplitNodeList(TopFrontLeftNodes, NodeList, FirstCorner, FirstCorner + HalfDistance);

	Octants[0] = MakeUnique<Octant>(
		FirstCorner,
		FirstCorner + HalfDistance,
		WorldLocation,
		TopFrontLeftNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

	//top front right
	FVector Octant2FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y, FirstCorner.Z);

	TArray<IOctreeInterface*> TopFrontRightNodes;

	SplitNodeList(TopFrontRightNodes, NodeList, Octant2FirstCorner, Octant2FirstCorner + HalfDistance);

	Octants[1] = MakeUnique<Octant>(
		Octant2FirstCorner,
		Octant2FirstCorner + HalfDistance,
		WorldLocation,
		TopFrontRightNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

	//top back left
	FVector Octant3FirstCorner = FVector(FirstCorner.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z);

	TArray<IOctreeInterface*> TopBackLeftNodes;

	SplitNodeList(TopBackLeftNodes, NodeList, Octant3FirstCorner, Octant3FirstCorner + HalfDistance);

	Octants[2] = MakeUnique<Octant>(
		Octant3FirstCorner,
		Octant3FirstCorner + HalfDistance,
		WorldLocation,
		TopBackLeftNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

	//top back right
	FVector Octant4FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z);

	TArray<IOctreeInterface*> TopBackRightNodes;

	SplitNodeList(TopBackRightNodes, NodeList, Octant4FirstCorner, Octant4FirstCorner + HalfDistance);

	Octants[3] = MakeUnique<Octant>(
		Octant4FirstCorner,
		Octant4FirstCorner + HalfDistance,
		WorldLocation,
		TopBackRightNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

	//bottom front left
	FVector Octant5FirstCorner = FVector(FirstCorner.X, FirstCorner.Y, FirstCorner.Z + HalfDistance.Z);

	TArray<IOctreeInterface*> BottomFrontLeftNodes;

	SplitNodeList(BottomFrontLeftNodes, NodeList, Octant5FirstCorner, Octant5FirstCorner + HalfDistance);

	Octants[4] = MakeUnique<Octant>(
		Octant5FirstCorner,
		Octant5FirstCorner + HalfDistance,
		WorldLocation,
		BottomFrontLeftNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

	//bottom front right
	FVector Octant6FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y, FirstCorner.Z + HalfDistance.Z);

	TArray<IOctreeInterface*> BottomFrontRightNodes;

	SplitNodeList(BottomFrontRightNodes, NodeList, Octant6FirstCorner, Octant6FirstCorner + HalfDistance);

	Octants[5] = MakeUnique<Octant>(
		Octant6FirstCorner,
		Octant6FirstCorner + HalfDistance,
		WorldLocation,
		BottomFrontRightNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

	//bottom back left
	FVector Octant7FirstCorner = FVector(FirstCorner.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z + HalfDistance.Z);

	TArray<IOctreeInterface*> BottomBackLeftNodes;

	SplitNodeList(BottomBackLeftNodes, NodeList, Octant7FirstCorner, Octant7FirstCorner + HalfDistance);

	Octants[6] = MakeUnique<Octant>(
		Octant7FirstCorner,
		Octant7FirstCorner + HalfDistance,
		WorldLocation,
		BottomBackLeftNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

	//bottom back right
	FVector Octant8FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z + HalfDistance.Z);

	TArray<IOctreeInterface*> BottomBackRightNodes;

	SplitNodeList(BottomBackRightNodes, NodeList, Octant8FirstCorner, Octant8FirstCorner + HalfDistance);

	Octants[7] = MakeUnique<Octant>(
		Octant8FirstCorner,
		Octant8FirstCorner + HalfDistance,
		WorldLocation,
		BottomBackRightNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

	subdevided = true;
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

bool Octant::IsWithinArea(const FVector& Location, const FVector& FCorner, const FVector& SCorner)
{
	FVector FirstWorldCorner = FCorner + WorldLocation;
	FVector SecondWorldCorner = SCorner + WorldLocation;

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

void Octant::SplitNodeList(TArray<IOctreeInterface*>& OctantNodeList, TArray<IOctreeInterface*>& TempNodeList, const FVector& FCorner, const FVector& SCorner)
{

	TArray<IOctreeInterface*> TempNodeListToRemove;

	//couldve been rangebased but pointless to change
	for (int i = 0; i < TempNodeList.Num(); i++) {
		if (IsWithinArea(TempNodeList[i]->GetPosition(), FCorner, SCorner)) {
			OctantNodeList.Add(TempNodeList[i]);
			TempNodeListToRemove.Add(TempNodeList[i]);
		}
	}

	for (IOctreeInterface*& node : TempNodeListToRemove) {
		TempNodeList.Remove(node);
	}
}