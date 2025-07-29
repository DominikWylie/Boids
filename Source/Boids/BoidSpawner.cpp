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
		World->GetTimerManager().SetTimer(MyTimerHandle, this, &ABoidSpawner::Spawn, 0.1f, true);
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

	//ill keep the octree getter but this was made just to avoid overloading the machine
	if (BoidCount > 400) {
		return;
	}

	if (World) {
		FRotator BoidRotation;

		BoidRotation.Yaw = FMath::RandRange(-180, 180);
		BoidRotation.Pitch = FMath::RandRange(-180, 180);
		BoidRotation.Roll = 0.f;

		AActor* SpawnedBoid = World->SpawnActor<ABoid>(BoidBlueprint, GetActorLocation(), BoidRotation);

		ABoid* Boid = Cast<ABoid>(SpawnedBoid);
		if (Boid) {
			Boid->initialise(
				Octree,
				FirstBounds,
				SecondBounds,
				CentreBounds,
				&ImGuiMods
			);
		}

		IOctreeInterface* BoidsInterface = Cast<IOctreeInterface>(SpawnedBoid);

		Octree->AddNode(BoidsInterface);
	}

	BoidCount++;
}

void ABoidSpawner::Imgui()
{
	if (PC->WasInputKeyJustPressed("P")) {
		FImGuiModule::Get().GetProperties().ToggleInput();
		ImGui::SetNextWindowCollapsed(false);
	}

	DrawDebugSphere(GetWorld(), CentreBounds, ImGuiMods.ProtectedRange, 10, FColor::Red);
	DrawDebugSphere(GetWorld(), CentreBounds, ImGuiMods.VisualRange, 10, FColor::Green);

	if (ImGui::Begin("When will you wear wigs", nullptr, ImGuiWindowFlags_MenuBar)) {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::Button("Back to game")) {
				FImGuiModule::Get().GetProperties().ToggleInput();
				ImGui::SetWindowCollapsed(true);
				PC->SetInputMode(FInputModeGameOnly());
			}
		}
		ImGui::EndMenuBar();

		ImGui::Text(std::to_string(BoidCount).c_str());

		ImGui::SliderFloat("Centering factor", &ImGuiMods.CenteringFactor, 0.f, 10.f);
		ImGui::SliderFloat("Matching factor", &ImGuiMods.MatchingFactor, 0.f, 10.f);
		ImGui::SliderFloat("Avoidance factor", &ImGuiMods.AvoidBoidsFactor, 0.f, 10.f);
		ImGui::SliderFloat("General turning speed", &ImGuiMods.GeneralTurningSpeed, 0.f, 10.f);
		ImGui::SliderFloat("Boids speed", &ImGuiMods.Speed, 0.f, 10000.f);

		ImGui::SliderFloat("Visual range", &ImGuiMods.VisualRange, 0.f, 3000.f);
		ImGui::SliderFloat("Protected range", &ImGuiMods.ProtectedRange, 0.f, 3000.f);
	}


	ImGui::End();
}

// Called every frame
void ABoidSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Imgui();


	//DrawDebugBox(World, ((SecondBounds - GetActorLocation()) + (FirstBounds - GetActorLocation())) / 2, (FirstBounds - SecondBounds) / 2, FColor::Green);
	DrawDebugBox(World, (SecondBounds + FirstBounds) * 0.5f, (FirstBounds - SecondBounds) * 0.5f, FColor::Green);
}
