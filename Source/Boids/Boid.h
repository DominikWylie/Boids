// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "OctreeInterface.h"

#include "Boid.generated.h"

UCLASS()
class BOIDS_API ABoid : public AActor, public IOctreeInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();

	UPROPERTY(EditAnywhere)
	float Speed = 700.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetPosition() const override;

	void Kill() override;

	void TempPause() override;

	void colourin() override;

	bool move = true;

	int32 NodeMatchIDTemp = 0;

	int32 id() override;

};
