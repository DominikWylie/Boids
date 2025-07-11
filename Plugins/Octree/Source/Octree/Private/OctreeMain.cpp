// Fill out your copyright notice in the Description page of Project Settings.

#include "OctreeMain.h"

#include "OctreeInterface.h"

// Sets default values
AOctreeMain::AOctreeMain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	PrimaryActorTick.bStartWithTickEnabled = true;

	World = GetWorld();

#if WITH_EDITOR
	bRunConstructionScriptOnDrag = true; //for visual updates while moving in editor
#endif
}

void AOctreeMain::AddNode(IOctreeInterface* Node)
{
	//check if node is in area, if not - ignore.
	
	//FVector NodePosition = Node->GetPosition();

	if (!IsWithinArea(Node->GetPosition())) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("node out of area"));
		return;
	}

	NodeList.Add(Node);


	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("node Added"));

	//check the max number of nodes

	//if (!subdevided) {
	//	SubdivideTree();
	//	subdevided = true;

	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("subdevided"));
	//}
}

void AOctreeMain::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	WorldLocation = GetActorLocation();
}

void AOctreeMain::DrawBox()
{
	if (!bBoundingBoxVisibiliy) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("invisible"));
		return;
	}

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Boundbox"));

	DrawDebugBox(World, ((SecondCorner + WorldLocation) + (FirstCorner + WorldLocation)) / 2, (FirstCorner - SecondCorner) / 2, FColor::Red);
}

void AOctreeMain::SubdivideTree()
{
	//if (subdevided) {
	//	return;
	//}

	FVector HalfDistance = (SecondCorner - FirstCorner) / 2;

	FVector makeItClearthe4ththisbetemp = FVector(50.f, 50.f, 50.f);

	//top front left
	Octants[0] = MakeUnique<Octant>(FirstCorner, FirstCorner + HalfDistance);

	//top front right
	FVector Octant2FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y, FirstCorner.Z);
	Octants[1] = MakeUnique<Octant>(Octant2FirstCorner, Octant2FirstCorner + HalfDistance);

	//top back left
	FVector Octant3FirstCorner = FVector(FirstCorner.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z);
	Octants[2] = MakeUnique<Octant>(Octant3FirstCorner, Octant3FirstCorner + HalfDistance);

	//top back right
	FVector Octant4FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z);
	Octants[3] = MakeUnique<Octant>(Octant4FirstCorner, Octant4FirstCorner + HalfDistance);



	//bottom front left
	FVector Octant5FirstCorner = FVector(FirstCorner.X, FirstCorner.Y, FirstCorner.Z + HalfDistance.Z);
	Octants[4] = MakeUnique<Octant>(Octant5FirstCorner, Octant5FirstCorner + HalfDistance);

	//bottom front right
	FVector Octant6FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y, FirstCorner.Z + HalfDistance.Z);
	Octants[5] = MakeUnique<Octant>(Octant6FirstCorner, Octant6FirstCorner + HalfDistance);

	//bottom back left
	FVector Octant7FirstCorner = FVector(FirstCorner.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z + HalfDistance.Z);
	Octants[6] = MakeUnique<Octant>(Octant7FirstCorner, Octant7FirstCorner + HalfDistance);

	//bottom back right
	FVector Octant8FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z + HalfDistance.Z);
	Octants[7] = MakeUnique<Octant>(Octant8FirstCorner, Octant8FirstCorner + HalfDistance);

	//Octants[7] = MakeUnique<Octant>(Octant7FirstCorner + makeItClearthe4ththisbetemp, (Octant7FirstCorner + HalfDistance) + makeItClearthe4ththisbetemp);


	//for (TUniquePtr<Octant>& octant : Octants) {
	//	octant->Subdivide();
	//}

	subdevided = true;

}

bool AOctreeMain::IsWithinArea(const FVector& Location)
{
	FVector FirstWorldCorner = FirstCorner + GetActorLocation();
	FVector SecondWorldCorner = SecondCorner + GetActorLocation();
	
	if (Location.X > FirstWorldCorner.X ||
		Location.Y > FirstWorldCorner.Y ||
		Location.Z > FirstWorldCorner.Z) {
		return false;
	}

	if (Location.X < SecondWorldCorner.X ||
		Location.Y < SecondWorldCorner.Y ||
		Location.Z < SecondWorldCorner.Z) {
		return false;
	}
	
	return true;
}

void AOctreeMain::RebuildTree()
{
	//all (boids) should be within the main at all times, if it leaves, have a "left" function that is called to the node

	//uint8 NodeCount = 0;


	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("node rebuild"));

	TArray<IOctreeInterface*> NodesToRemove;

	for (IOctreeInterface*& Node : NodeList) {

		FVector loc = FirstCorner + GetActorLocation();

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(2, 15.0f, FColor::Blue, FString::Printf(TEXT("X %f, Y %f, Z %f, "), loc.X, loc.Y, loc.Z));

		if (!IsWithinArea(Node->GetPosition())) {
			//kill node for now
			 
			NodesToRemove.Add(Node);
			
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("node killed"));
		}
	}

	for (IOctreeInterface*& Node : NodesToRemove) {
		NodeList.Remove(Node);
		Node->Kill();
	}

}

//FOR SOME REASON FOR UPDATING IN EDITOR TICK YOU HAVE TO PRESS RECOMPILE (LIVE CODING OFF) 3 TIMES FOR IT TO UPDATE: I AM NOT ONE TO QUESTION THE COMPILER GODS
//may be just a problem with the unreal vesion? in game tick only updates with 3 compiles too i still think its cos im using editor tick
void AOctreeMain::Tick(float DeltaTime)
{
	if (IsRunningGame()) {
		Super::Tick(DeltaTime);
	} 

	RebuildTree();

	//UE_LOG(LogTemp, Warning, TEXT("editor tick"));

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("tick"));

#if WITH_EDITOR
	DrawBox();
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("tick within WITH_EDITOR"));
		if (subdevided) {
			for (TUniquePtr<Octant>& octant : Octants) {
				if (octant) octant->Tick(World, WorldLocation);
			}
		}
#endif
}

void AOctreeMain::BeginPlay()
{
	WorldLocation = GetActorLocation();

	World = GetWorld();

	if (FirstCorner.X < SecondCorner.X) {
		int32 temp = FirstCorner.X;
		FirstCorner.X = SecondCorner.X;
		SecondCorner.X = temp;

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("swapped x"));
	}

	if (FirstCorner.Y < SecondCorner.Y) {
		int32 temp = FirstCorner.Y;
		FirstCorner.Y = SecondCorner.Y;
		SecondCorner.Y = temp;

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("swapped y"));
	}

	if (FirstCorner.Z < SecondCorner.Z) {
		int32 temp = FirstCorner.Z;
		FirstCorner.Z = SecondCorner.Z;
		SecondCorner.Z = temp;

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("swapped z"));
	}
}

