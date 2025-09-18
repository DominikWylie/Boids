#pragma once

#include "CoreMinimal.h"

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "ImGuiModifier.generated.h"

class AOctreeMain;

USTRUCT()
struct FBoidPreset {
	//keep the default values above for safety
	//even tho they are replaces with the saved ones

	GENERATED_BODY()
	
	UPROPERTY()
	float CenteringFactor = 3.3f;
	UPROPERTY()
	float MatchingFactor = 4.5f;
	UPROPERTY()
	float AvoidBoidsFactor = 4.7f;
	UPROPERTY()
	float GeneralTurningSpeed = 0.6f;
	UPROPERTY()
	float Speed = 2507.f;
	UPROPERTY()
	float ProtectedRange = 250.f;
	UPROPERTY()
	float VisualRange = 1160.f;
	UPROPERTY()
	int32 BoidMax = 200;
};

USTRUCT()
struct FImGuiModifier
{
	GENERATED_BODY()
	
	FImGuiModifier();

	void BeginPlay(UWorld* World);
	
	void RenderImGui();
	
	//set and forget, this pointer changes when presets are swapped
	std::shared_ptr<const FBoidPreset> GetCurrentPreset() const {return currentPresetPtr;}

private:
	
	void Save();
	void Load();

	APlayerController* PC;

	TObjectPtr<AOctreeMain> Octree;
	
	bool Initialised = false;

	FString SaveSlotName = "BoidPresetSave1";

	std::string directory = "Content/Boids/ValuePresets";
	
	TArray<FName> NamesArr;
	TMap<FName, FBoidPreset> Presets;
	int32 CurrentItem = 0;

	std::shared_ptr<FBoidPreset> currentPresetPtr;

};

