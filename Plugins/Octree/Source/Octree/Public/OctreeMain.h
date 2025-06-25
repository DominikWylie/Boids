// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Octant.h"

#include "OctreeMain.generated.h"

class IOctreeInterface;

UCLASS()
class OCTREE_API AOctreeMain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOctreeMain();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = true))
	FVector FirstCorner = FVector(50.f, 50.f, 50.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = true))
	FVector SecondCorner = FVector(-50.f, -50.f, 50.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBoundingBoxVisibiliy = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 NodesPerOctant = 1;

	void AddNode(IOctreeInterface* Node);

protected:

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

private:
	void DrawBox();

	void SubdivideTree();

	bool IsWithinArea(const FVector& Location);

	void RebuildTree();

	FVector WorldLocation;
	UWorld* World;

	TArray<IOctreeInterface*> NodeList;

	TUniquePtr<Octant> Octants[8];

	bool subdevided = false;

#if WITH_EDITOR
	//virtual void Tick(float DeltaTime) override;
	virtual bool ShouldTickIfViewportsOnly() const override { return true; }
#endif

	//todo
	//create custom AABB collision for axis alighned efficiancy
};
