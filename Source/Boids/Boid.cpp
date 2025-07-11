// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	//SetActorLocation(GetActorLocation() + (GetActorForwardVector() * Speed * DeltaTime));
	SetActorLocation(GetActorLocation() + (GetActorUpVector() * Speed * DeltaTime));

	FVector loc = GetActorLocation();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Green, FString::Printf(TEXT("X %f, Y %f, Z %f, "), loc.X, loc.Y, loc.Z));
}

FVector ABoid::GetPosition() const
{
	return GetActorLocation();
}

void ABoid::Kill()
{
	Destroy();
}
