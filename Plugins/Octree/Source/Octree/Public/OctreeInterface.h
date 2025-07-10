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

	//is killed wen leaving tree area (maybe change to teleport to othewr side? ideally the boids will turn around)
	virtual void Kill() = 0;
};
