// Fill out your copyright notice in the Description page of Project Settings.

#include "BoidSpawner.h"

#include "Boid.h"
#include "OctreeMain.h"
#include "Kismet/GameplayStatics.h"
//#include "OctreeInterface.h"

// Sets default values
ABoidSpawner::ABoidSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoidSpawner::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();

	if (World) {
		World->GetTimerManager().SetTimer(MyTimerHandle, this, &ABoidSpawner::Spawn, 0.1f, true);
	}

	Octree = Cast<AOctreeMain>(UGameplayStatics::GetActorOfClass(World, AOctreeMain::StaticClass()));
}

void ABoidSpawner::Spawn()
{
	if (!IsValid(Octree)) {
		UE_LOG(LogTemp, Error, TEXT("No Octree to send to. [%s:%d] in %s"),
			TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__));
		return;
	}
	
	//ill keep the octree getter but this was made just to avoid overloading the machine
	//if (Octree->GetNodeNum() > 100) {
	//	return;
	//}

	if (World) {
		FRotator BoidRotation;

		BoidRotation.Yaw = FMath::RandRange(-90, 90);
		BoidRotation.Pitch = FMath::RandRange(-90, 90);
		//roll doesnt need changed (when using moidels may want to anchor it somehow)
		
		AActor* SpawnedBoid = World->SpawnActor<ABoid>(BoidBlueprint, GetActorLocation(), BoidRotation);

		IOctreeInterface* BoidsInterface = Cast<IOctreeInterface>(SpawnedBoid);

		Octree->AddNode(BoidsInterface);

		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Hit"));
	}
}

// Called every frame
void ABoidSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
