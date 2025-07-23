// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Octant.h"

#include "OctreeMain.generated.h"

class IOctreeInterface;

//this was giving the user the choice to update manually but im gona keep it simple for now
//UENUM(BlueprintType)
//enum class EUdateMode : uint8 
//{
//	PerTick UMETA(DisplayName = "Update automticly evrry tick"),
//	Manual 	UMETA(DisplayName = "Update Manually using "),
//};

UCLASS()
class OCTREE_API AOctreeMain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOctreeMain();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Meta = (MakeEditWidget = true))
	FVector FirstCorner = FVector(50.f, 50.f, 50.f);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Meta = (MakeEditWidget = true))
	FVector SecondCorner = FVector(-50.f, -50.f, 50.f);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bBoundingBoxVisibiliy = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	uint8 MaxNodesPerOctant = 1;

	void AddNode(IOctreeInterface* Node);

	uint32 GetNodeNum() const { return NodeList.Num(); }

	//overload if other shapes needed
	TArray<IOctreeInterface*> NodeQuery(const FVector& Centre, float Extent);

protected:

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

private:
	void DrawBox();

	void SubdivideTree();

	bool IsWithinArea(const FVector& Location);
	bool IsWithinArea(const FVector& Location, const FVector& FirstCorner, const FVector& SecondCorner);

	void SplitNodeList(TArray<IOctreeInterface*>& OctantNodeList, TArray<IOctreeInterface*>& TempNodeList, const FVector& FirstCorner, const FVector& SecondCorner);

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
