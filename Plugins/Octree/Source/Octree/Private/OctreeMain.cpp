// Fill out your copyright notice in the Description page of Project Settings.

#include "OctreeMain.h"

#include "OctreeInterface.h"

// Sets default values
AOctreeMain::AOctreeMain()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

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
		Node->Kill();
		return;
	}

	NodeList.Add(Node);

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("node Added"));

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
	if (bBoundingBoxVisibiliy) {
		DrawDebugBox(World, ((SecondCorner + WorldLocation) + (FirstCorner + WorldLocation)) / 2, (FirstCorner - SecondCorner) / 2, FColor::Red);
	}
}

void AOctreeMain::SubdivideTree()
{
	FVector HalfDistance = (SecondCorner - FirstCorner) / 2;

	TArray<IOctreeInterface*> TempNodesList = NodeList;
	//origonaly was nodelist just passed in (if broke)

	//top front left
	TArray<IOctreeInterface*> TopFrontLeftNodes;

	SplitNodeList(TopFrontLeftNodes, TempNodesList, FirstCorner, FirstCorner + HalfDistance);

	Octants[0] = MakeUnique<Octant>(
		FirstCorner,
		FirstCorner + HalfDistance,
		WorldLocation,
		TopFrontLeftNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

	//top front right
	FVector Octant2FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y, FirstCorner.Z);

	TArray<IOctreeInterface*> TopFrontRightNodes;

	SplitNodeList(TopFrontRightNodes, TempNodesList, Octant2FirstCorner, Octant2FirstCorner + HalfDistance);

	Octants[1] = MakeUnique<Octant>(
		Octant2FirstCorner,
		Octant2FirstCorner + HalfDistance,
		WorldLocation,
		TopFrontRightNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

	//top back left
	FVector Octant3FirstCorner = FVector(FirstCorner.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z);

	TArray<IOctreeInterface*> TopBackLeftNodes;

	SplitNodeList(TopBackLeftNodes, TempNodesList, Octant3FirstCorner, Octant3FirstCorner + HalfDistance);

	Octants[2] = MakeUnique<Octant>(
		Octant3FirstCorner,
		Octant3FirstCorner + HalfDistance,
		WorldLocation,
		TopBackLeftNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

	//top back right
	FVector Octant4FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z);

	TArray<IOctreeInterface*> TopBackRightNodes;

	SplitNodeList(TopBackRightNodes, TempNodesList, Octant4FirstCorner, Octant4FirstCorner + HalfDistance);

	Octants[3] = MakeUnique<Octant>(
		Octant4FirstCorner,
		Octant4FirstCorner + HalfDistance,
		WorldLocation,
		TopBackRightNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

	//bottom front left
	FVector Octant5FirstCorner = FVector(FirstCorner.X, FirstCorner.Y, FirstCorner.Z + HalfDistance.Z);

	TArray<IOctreeInterface*> BottomFrontLeftNodes;

	SplitNodeList(BottomFrontLeftNodes, TempNodesList, Octant5FirstCorner, Octant5FirstCorner + HalfDistance);

	Octants[4] = MakeUnique<Octant>(
		Octant5FirstCorner,
		Octant5FirstCorner + HalfDistance,
		WorldLocation,
		BottomFrontLeftNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

	//bottom front right
	FVector Octant6FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y, FirstCorner.Z + HalfDistance.Z);

	TArray<IOctreeInterface*> BottomFrontRightNodes;

	SplitNodeList(BottomFrontRightNodes, TempNodesList, Octant6FirstCorner, Octant6FirstCorner + HalfDistance);

	Octants[5] = MakeUnique<Octant>(
		Octant6FirstCorner,
		Octant6FirstCorner + HalfDistance,
		WorldLocation,
		BottomFrontRightNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

	//bottom back left
	FVector Octant7FirstCorner = FVector(FirstCorner.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z + HalfDistance.Z);

	TArray<IOctreeInterface*> BottomBackLeftNodes;

	SplitNodeList(BottomBackLeftNodes, TempNodesList, Octant7FirstCorner, Octant7FirstCorner + HalfDistance);

	Octants[6] = MakeUnique<Octant>(
		Octant7FirstCorner,
		Octant7FirstCorner + HalfDistance,
		WorldLocation,
		BottomBackLeftNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

	//bottom back right
	FVector Octant8FirstCorner = FVector(FirstCorner.X + HalfDistance.X, FirstCorner.Y + HalfDistance.Y, FirstCorner.Z + HalfDistance.Z);

	TArray<IOctreeInterface*> BottomBackRightNodes;

	SplitNodeList(BottomBackRightNodes, TempNodesList, Octant8FirstCorner, Octant8FirstCorner + HalfDistance);

	Octants[7] = MakeUnique<Octant>(
		Octant8FirstCorner,
		Octant8FirstCorner + HalfDistance,
		WorldLocation,
		BottomBackRightNodes,
		MaxNodesPerOctant,
		World,
		bBoundingBoxVisibiliy
	);

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

bool AOctreeMain::IsWithinArea(const FVector& Location, const FVector& FCorner, const FVector& SCorner)
{
	FVector FirstWorldCorner = FCorner + GetActorLocation();
	FVector SecondWorldCorner = SCorner + GetActorLocation();

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

void AOctreeMain::SplitNodeList(TArray<IOctreeInterface*>& OctantNodeList, TArray<IOctreeInterface*>& TempNodeList, const FVector& FCorner, const FVector& SCorner)
{

	TArray<IOctreeInterface*> TempNodeListToRemove;

	//couldve been rangebased but pointless to change
	for (int32 i = 0; i < TempNodeList.Num(); i++) {
		if (IsWithinArea(TempNodeList[i]->GetPosition(), FCorner, SCorner)) {
			OctantNodeList.Add(TempNodeList[i]);
			TempNodeListToRemove.Add(TempNodeList[i]);
		}
	}

	for (IOctreeInterface*& node : TempNodeListToRemove) {
		TempNodeList.Remove(node);
	}
}

void AOctreeMain::RebuildTree()
{

	TArray<IOctreeInterface*> NodesToRemove;

	for (IOctreeInterface*& Node : NodeList) {

		//check if all nodes are inside
		if (!IsWithinArea(Node->GetPosition())) {
			//kill node for now

			NodesToRemove.Add(Node);
		}
	}

	for (IOctreeInterface*& Node : NodesToRemove) {
		NodeList.Remove(Node);
		Node->Kill();
	}

	//if exeeds max nodes subdivide and pass nodes to them to do thier thing

	if (NodeList.Num() > MaxNodesPerOctant) {
		SubdivideTree();
	}
	else if (subdevided) {
		//if the suboctants are no longer wanted
		subdevided = false;

		for (TUniquePtr<Octant>& octant : Octants) {
			octant.Reset();
		}
	}
}

//FOR SOME REASON FOR UPDATING IN EDITOR TICK YOU HAVE TO PRESS RECOMPILE (LIVE CODING OFF) 3 TIMES FOR IT TO UPDATE: I AM NOT ONE TO QUESTION THE COMPILER GODS
//may be just a problem with the unreal vesion? in game tick only updates with 3 compiles too i still think its cos im using editor tick
void AOctreeMain::Tick(float DeltaTime)
{
	if (GetWorld() && GetWorld()->IsGameWorld()) {
		Super::Tick(DeltaTime);
		RebuildTree();

		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Yellow, TEXT("subdivided in main"));

		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(2, 15.0f, FColor::Yellow, FString::Printf(TEXT("node amount: %d, MaxNodesPerOctant %d"), NodeList.Num(), MaxNodesPerOctant));
	}

	//UE_LOG(LogTemp, Warning, TEXT("editor tick"));



#if WITH_EDITOR
	DrawBox();

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("tick within WITH_EDITOR"));
	//if (subdevided) {
	//	for (TUniquePtr<Octant>& octant : Octants) {
	//		if (octant) octant->Tick();
	//	}
	//}
#endif
}

void AOctreeMain::BeginPlay()
{
	SetActorTickEnabled(true);

	WorldLocation = GetActorLocation();

	World = GetWorld();

	if (FirstCorner.X < SecondCorner.X) {
		int32 temp = FirstCorner.X;
		FirstCorner.X = SecondCorner.X;
		SecondCorner.X = temp;

		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("swapped x"));
	}

	if (FirstCorner.Y < SecondCorner.Y) {
		int32 temp = FirstCorner.Y;
		FirstCorner.Y = SecondCorner.Y;
		SecondCorner.Y = temp;

		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("swapped y"));
	}

	if (FirstCorner.Z < SecondCorner.Z) {
		int32 temp = FirstCorner.Z;
		FirstCorner.Z = SecondCorner.Z;
		SecondCorner.Z = temp;

		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("swapped z"));
	}
}

TArray<IOctreeInterface*> AOctreeMain::NodeQuery(const FVector& Centre, float Extent)
{
	//second is min, first is max

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("query hit"));

	int32 DistanceMin = 0;

	FVector FirstWorldCorner = FirstCorner + WorldLocation;
	FVector SecondWorldCorner = SecondCorner + WorldLocation;

	for (int32 i = 0; i < 3; i++) {
		if (Centre[i] < SecondWorldCorner[i]) DistanceMin += FMath::Square(Centre[i] - SecondWorldCorner[i]);
		else if (Centre[i] > FirstWorldCorner[i]) DistanceMin += FMath::Square(Centre[i] - FirstWorldCorner[i]);
	}

	if (!(DistanceMin <= FMath::Square(Extent))) {
		//if out of main send back an empty array
		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("not hit octant"));
		return TArray<IOctreeInterface*>();
	}

	if (!subdevided) {
		//end of tree
		return NodeList;
	}

	TArray<IOctreeInterface*> QuerydNodeList;

	for (const TUniquePtr<Octant>& octant : Octants) {
		//now do this function in the next that i havent set up yet
		QuerydNodeList += octant->NodeQuery(Centre, Extent);
	}

	for (IOctreeInterface* node : QuerydNodeList) {
		node->colourin();
	}

	return QuerydNodeList;
}

void AOctreeMain::pauseNodes()
{
	for (IOctreeInterface*& Node : NodeList) {
		Node->TempPause();
	}
}
