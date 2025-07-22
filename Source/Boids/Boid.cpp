// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NodeMatchIDTemp = FMath::RandRange(1, 1000);

}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (move) {
		SetActorLocation(GetActorLocation() + (GetActorUpVector() * Speed * DeltaTime));
	}
	else {
		//DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 70), FString::FromInt(NodeMatchIDTemp), NULL, FColor::Red, -1.f);
	}

	FVector loc = GetActorLocation();

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Green, FString::Printf(TEXT("X %f, Y %f, Z %f, "), loc.X, loc.Y, loc.Z));
}

FVector ABoid::GetPosition() const
{
	return GetActorLocation();
}

void ABoid::Kill()
{
	Destroy();
}

void ABoid::TempPause()
{
	move = false;
}

void ABoid::colourin()
{
	float radius = 100;
	FVector location = FVector(0, 0, 1200);
	DrawDebugSphere(GetWorld(), GetActorLocation(), radius, 15, FColor::Purple, false);
}

int32 ABoid::id()
{
	return NodeMatchIDTemp;
}
