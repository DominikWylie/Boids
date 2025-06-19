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
}
