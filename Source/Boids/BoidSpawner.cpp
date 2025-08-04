// Fill out your copyright notice in the Description page of Project Settings.

#include "BoidSpawner.h"

#include "Boid.h"
#include "OctreeMain.h"
#include "Kismet/GameplayStatics.h"
#include "imgui.h"
#include "ImGuiInputHandler.h"
#include "ImGuiModule.h"
#include <string>
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

	PC = GetWorld()->GetFirstPlayerController();

	if (World) {
		World->GetTimerManager().SetTimer(MyTimerHandle, this, &ABoidSpawner::Spawn, 0.01f, true);
	}

	Octree = Cast<AOctreeMain>(UGameplayStatics::GetActorOfClass(World, AOctreeMain::StaticClass()));

	if (Octree) {

		Octree->GetWorldCorners(FirstBounds, SecondBounds, CentreBounds);

		FirstBounds -= FVector(BoundsBuffer);
		SecondBounds += FVector(BoundsBuffer);
	}
}

void ABoidSpawner::Spawn()
{
	if (!IsValid(Octree)) {
		UE_LOG(LogTemp, Error, TEXT("No Octree to send to. [%s:%d] in %s"),
			TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__));
		return;
	}

	if (Octree->GetNodeNum() > ImGuiMods.BoidMax) {
		return;
	}

	if (World) {
		FRotator BoidRotation;

		BoidRotation.Yaw = FMath::RandRange(-180, 180);
		BoidRotation.Pitch = FMath::RandRange(-180, 180);
		BoidRotation.Roll = 0.f;

		FVector SpawnLocation;
		SpawnLocation.X = FMath::RandRange(Octree->SecondCorner.X, Octree->FirstCorner.X);
		SpawnLocation.Y = FMath::RandRange(Octree->SecondCorner.Y, Octree->FirstCorner.Y);
		SpawnLocation.Z = FMath::RandRange(Octree->SecondCorner.Z, Octree->FirstCorner.Z);


		AActor* SpawnedBoid = World->SpawnActor<ABoid>(BoidBlueprint, SpawnLocation, BoidRotation);

		ABoid* Boid = Cast<ABoid>(SpawnedBoid);
		if (IsValid(Boid)) {
			Boid->initialise(
				Octree,
				FirstBounds,
				SecondBounds,
				CentreBounds,
				&ImGuiMods
			);

			if (!IsValid(BoidWatch)) {
				BoidWatch = Boid;
			}
		}

		IOctreeInterface* BoidsInterface = Cast<IOctreeInterface>(SpawnedBoid);

		Octree->AddNode(BoidsInterface);
	}
}

void ABoidSpawner::Imgui()
{
	if (PC->WasInputKeyJustPressed("P")) {
		FImGuiModule::Get().GetProperties().ToggleInput();
		ImGui::SetNextWindowCollapsed(false);
	}

	ImGui::SetNextWindowBgAlpha(0.3f);

	if (ShowVisualRangeSpheres) {
		DrawDebugSphere(GetWorld(), CentreBounds, ImGuiMods.ProtectedRange, 10, FColor::Red);
		DrawDebugSphere(GetWorld(), CentreBounds, ImGuiMods.VisualRange, 10, FColor::Green);
	}

	if (ImGui::Begin("When will you wear wigs", nullptr, ImGuiWindowFlags_MenuBar)) {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::Button("Back to game")) {
				FImGuiModule::Get().GetProperties().ToggleInput();
				ImGui::SetWindowCollapsed(true);
				PC->SetInputMode(FInputModeGameOnly());
			}

			if (ImGui::Button("Save")) {
				ImGuiMods.Save();
			}

			if (ImGui::Button("Load")) {
				ImGuiMods.Load();
			}

			if (ImGui::Button("Toggle visual range")) {
				ShowVisualRangeSpheres = !ShowVisualRangeSpheres;
			}
		}
		ImGui::EndMenuBar();

		ImGui::Text(std::to_string(Octree->GetNodeNum()).c_str());

		ImGui::SliderFloat("Centering factor", &ImGuiMods.CenteringFactor, 0.f, 10.f);
		ImGui::SliderFloat("Matching factor", &ImGuiMods.MatchingFactor, 0.f, 100.f);
		ImGui::SliderFloat("Avoidance factor", &ImGuiMods.AvoidBoidsFactor, 0.f, 10.f);
		ImGui::SliderFloat("General turning speed", &ImGuiMods.GeneralTurningSpeed, 0.f, 10.f);
		ImGui::SliderFloat("Boids speed", &ImGuiMods.Speed, 0.f, 10000.f);

		ImGui::SliderFloat("Visual range", &ImGuiMods.VisualRange, 0.f, 10000.f);
		ImGui::SliderFloat("Protected range", &ImGuiMods.ProtectedRange, 0.f, 7000.f);

		if (ImGui::SliderInt("Boid max", &ImGuiMods.BoidMax, 0.f, 1000.f)) {
			Octree->ReduceNodes(ImGuiMods.BoidMax);
		}
	}


	ImGui::End();
}

// Called every frame
void ABoidSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Imgui();

	//if (GEngine && IsValid(BoidWatch))
	//	GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Green, FString::Printf(TEXT("boidwatch speed: %f"), BoidWatch->GetSpeed()));

	//DrawDebugBox(World, ((SecondBounds - GetActorLocation()) + (FirstBounds - GetActorLocation())) / 2, (FirstBounds - SecondBounds) / 2, FColor::Green);
	DrawDebugBox(World, (SecondBounds + FirstBounds) * 0.5f, (FirstBounds - SecondBounds) * 0.5f, FColor::Green);
}
