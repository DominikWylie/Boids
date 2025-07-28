// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "OctreeMain.h"
#include "BoidSpawner.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NodeMatchIDTemp = FMath::RandRange(1, 1000);

}

void ABoid::initialise(const TObjectPtr<AOctreeMain>& Otree, const FVector& FBounds, const FVector& Sbounds, const FVector& CBounds)
{
	Octree = Otree;
	UpperBounds = FBounds;
	LowerBounds = Sbounds;
	CentreOfBounds = CBounds;
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

	CalculateTrajectory(Octree->NodeQuery(GetActorLocation(), VisualRange), DeltaTime);
	
	if (move) {
		//rotate then translate
		SetActorLocation(GetActorLocation() + (GetActorForwardVector() * Speed * DeltaTime));
	}
}

FVector ABoid::GetPosition() const
{
	return GetActorLocation();
}

void ABoid::Kill()
{
	Destroy();
}

void ABoid::CalculateTrajectory(TArray<IOctreeInterface*> Boids, float dt)
{
	FRotator Rotation = GetActorRotation();

	FVector Location = GetActorLocation();

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Yellow, FString::Printf(TEXT("World delta for current frame equals %f"), GetWorld()->TimeSeconds));
	
	//if (140.f > 7860.f || -290.f > 8290.f) {
	//	int test = 89035;
	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("shithead!"));
	//}

	//FRotator test = UKismetMathLibrary::findlookat

	//DrawDebugSphere(GetWorld(), CentreOfBounds, 150, 10, FColor::Emerald);

	if (Location.X > UpperBounds.X ||
		Location.Y > UpperBounds.Y ||
		Location.Z > UpperBounds.Z ||
		Location.X < LowerBounds.X ||
		Location.Y < LowerBounds.Y ||
		Location.Z < LowerBounds.Z) 
	{
		FVector TargetDirection = CentreOfBounds - GetActorLocation();
		//TargetDirection.Normalize();

		FQuat CurrentQuat = GetActorQuat();
		FQuat TargetQuat = TargetDirection.Rotation().Quaternion();

		FQuat NewQuat = FQuat::Slerp(CurrentQuat, TargetQuat, RotationSpeed * dt);

		NewQuat.Normalize();

		//SetActorRotation(NewQuat);


		SetActorRotation(TargetQuat);


		//FRotator testro = FRotator::ZeroRotator;

		//testro.X = -testro.X;
		//testro.Y = -testro.Y;
		//testro.Z = -testro.Z;

		//testro.Yaw = -testro.Yaw;
		//testro.Pitch = -testro.Pitch;
		//testro.Roll = -testro.Roll
			;

		//SetActorRotation(testro);


		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("moving"));

		//FQuat TargetRotator = TargetDirection.Rotation().Quaternion();


		//FQuat TargetRotator = TargetDirection.Rotation().Quaternion();
		//FQuat TargetQuat = Targettotator




		//FVector CurrentLocation = GetActorLocation();
		
		//FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, BoundsCentre);

		//TargetRotation.Roll = 0.f;

		//FRotationMatrix::MakeFrom

		//FRotator SmoothRotation = FMath::RInterpConstantTo(GetActorRotation() )

		//FQuat test
	}
	
	
	//if (Location.X > UpperBounds.X || Location.Y > UpperBounds.Y) {
	//	Rotation.Yaw -= TurnFactor;
	//}

	//if (Location.Z > UpperBounds.Z) {
	//	Rotation.Pitch -= TurnFactor;
	//}

	//if (Location.X < LowerBounds.X || Location.Y < LowerBounds.Y) {
	//	Rotation.Yaw += TurnFactor;
	//}

	//if (Location.Z < LowerBounds.Z) {
	//	Rotation.Pitch += TurnFactor;
	//}

	//SetActorRotation(Rotation);


	//FVector DisplacementToOtherBoid;

	//FVector
	//	CloseRangeDisplacement = FVector::ZeroVector,
	//	re = FVector::ZeroVector,
	//	ro = FVector::ZeroVector;

	//float NeighboringBoids = 0;

	//for (IOctreeInterface*& Boid : Boids) {

	//	DisplacementToOtherBoid = GetActorLocation() - Boid->GetPosition();


	//	if (DisplacementToOtherBoid.SizeSquared() < FMath::Square(VisualRange)) {

	//		float DisplacementToOtherBoidSquared = DisplacementToOtherBoid.SizeSquared();

	//		if (DisplacementToOtherBoidSquared < FMath::Square(ProtectedRange)) {
	//			CloseRangeDisplacement += DisplacementToOtherBoid;
	//		}
	//		else {
	//			//in visual but not protected

	//		}
	//	}
	//}

	////for (int i = 0; i < Boids.Num(); i++) {

	////	for (int j = 0; j < Boids.Num(); j++) {
	////		if (i == j) continue;

	////		Displacement = GetActorLocation - Boids[]
	////	}
	////}
}
