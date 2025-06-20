// Fill out your copyright notice in the Description page of Project Settings.

#include "OctreeMain.h"

#include "OctreeInterface.h"

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

	FVector HalfDistance = (SecondCorner - FirstCorner) / 2;

	//FVector makeItClearthe4ththisbetemp = FVector(20.f, 20.f, 20.f);

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
	Octants[7] = MakeUnique<Octant>(Octant7FirstCorner, Octant7FirstCorner + HalfDistance);
}

void AOctreeMain::Tick(float DeltaTime)
{
	if (IsRunningGame()) {
		Super::Tick(DeltaTime);
	}

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("ticc"));

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

void AOctreeMain::BeginPlay()
{

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

