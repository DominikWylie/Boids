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
		const uint16& MaxNodesPerOctant,
		UWorld* World
	);

	~Octant();

	void Tick(/*UWorld* World*/);

	void SubdivideTree();

private:

	bool IsWithinArea(const FVector& Location);

	UWorld* World;

	TArray<IOctreeInterface*> NodeList;

	uint16 MaxNodesPerOctant;

	FVector FirstCorner;
	FVector SecondCorner;

	FVector WorldLocation;

	bool subdevided = false;

	TUniquePtr<Octant> Octants[8];
};
