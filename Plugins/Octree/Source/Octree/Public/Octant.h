// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class IOctreeInterface;

/**
 * 
 */
class OCTREE_API Octant
{
public:
	Octant(
		const FVector& FirstCorner, 
		const FVector& SecondCorner, 
		const FVector& WorldLocation, 
		const TArray<IOctreeInterface*>& NodeList, 
		const uint16 MaxNodesPerOctant,
		UWorld* World,
		const bool bBoundingBoxVisibiliy
	);

	~Octant();

	TArray<IOctreeInterface*> NodeQuery(const FVector& Centre, float Extent);

	//void Tick(/*UWorld* World*/);

private:

	void SubdivideTree();

	bool IsWithinArea(const FVector& Location);
	bool IsWithinArea(const FVector& Location, const FVector& FirstCorner, const FVector& SecondCorner);

	void SplitNodeList(TArray<IOctreeInterface*>& OctantNodeList, TArray<IOctreeInterface*>& TempNodeList, const FVector& FirstCorner, const FVector& SecondCorner);

	UWorld* World;

	TArray<IOctreeInterface*> NodeList;

	const uint16 MaxNodesPerOctant;

	const bool bBoundingBoxVisibiliy;

	FVector FirstCorner;
	FVector SecondCorner;

	FVector WorldLocation;

	bool subdevided = false;

	TUniquePtr<Octant> Octants[8];

	FColor BoxColour = FColor::Red;
};
