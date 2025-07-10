// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class OCTREE_API Octant
{
public:
	Octant(FVector FirstCorner, FVector SecondCorner);
	~Octant();

	void Tick(UWorld* World, FVector WorldLocation);

	void Subdivide();

private:

	FVector FirstCorner;
	FVector SecondCorner;

	bool subdevided = false;

	TUniquePtr<Octant> Octants[8];
};
