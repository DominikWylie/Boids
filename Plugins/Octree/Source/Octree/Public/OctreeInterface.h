// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OctreeInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UOctreeInterface : public UInterface
{
	GENERATED_BODY()
};

class IOctreeInterface 
{
	GENERATED_BODY()

public:
	virtual FVector GetPosition() const = 0;

	//Kill node is called (and removed from array) when it has left tree boundary
	virtual void Kill() = 0;

	virtual void TempPause() = 0;

	virtual void colourin() = 0;

	virtual int32 id() = 0;
};
