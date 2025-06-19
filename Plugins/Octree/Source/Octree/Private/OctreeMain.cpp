// Fill out your copyright notice in the Description page of Project Settings.

#include "OctreeInterface.h"

#include "OctreeMain.h"

// Sets default values
AOctreeMain::AOctreeMain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

#if WITH_EDITOR
	bRunConstructionScriptOnDrag = true; //for visual updates while moving in editor
#endif
}

void AOctreeMain::AddNode(IOctreeInterface* Node)
{
	//NodeList.Add(Node.GetInterface());
	NodeList.Add(Node);


	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("node Added"));

	if (!subdevided) {
		SubdivideTree();
		subdevided = true;
	}
}

void AOctreeMain::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AOctreeMain::DrawBox()
{
	if (!bBoundingBoxVisibiliy) {
		return;
	}

	FVector WorldLoc = GetActorLocation();

	DrawDebugBox(GetWorld(), ((SecondCorner + WorldLoc) + (FirstCorner + WorldLoc)) / 2, (FirstCorner - SecondCorner) / 2, FColor::Red);
}

void AOctreeMain::SubdivideTree()
{
	FVector WorldLoc = GetActorLocation();

	Octants[0] = MakeUnique<Octant>(FirstCorner, SecondCorner - (FirstCorner / 2));
	Octants[1] = MakeUnique<Octant>(FirstCorner - (SecondCorner / 2), SecondCorner);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("subdivide"));
}

void AOctreeMain::Tick(float DeltaTime)
{
	if (IsRunningGame()) {
		Super::Tick(DeltaTime);
	}

#if WITH_EDITOR
		DrawBox();

		if (subdevided) {

			//shouldnt have these in tick future problem
			UWorld* world = GetWorld();
			FVector WorldLoc = GetActorLocation();

			for (TUniquePtr<Octant>& octant : Octants) {
				octant->Tick(world, WorldLoc);
			}
		}
#endif
}

