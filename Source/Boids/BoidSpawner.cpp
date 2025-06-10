// Fill out your copyright notice in the Description page of Project Settings.

#include "Boid.h"
#include "BoidSpawner.h"

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
		World->GetTimerManager().SetTimer(MyTimerHandle, this, &ABoidSpawner::SpawnTimer, 1.0f, true);
	}
}

void ABoidSpawner::SpawnTimer()
{
	if (World) {
		AActor* SpawnedActor = World->SpawnActor<ABoid>(BoidBlueprint, GetActorLocation(), FRotator::ZeroRotator);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Hit"));
	}
}

// Called every frame
void ABoidSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

