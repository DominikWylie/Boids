#include "ImGuiModifier.h"

// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <filesystem>

#include "BoidPresetSaves.h"
#include "Kismet/GameplayStatics.h"

FImGuiModifier::FImGuiModifier()
{

		
	//load all data
			
	//this is a copy of the object - whech keeps the code safe, ill need to decide how to handle memory of modified vars
	//maybe save them to the map and add a note to the imgui window mentioning unsaved changes,
	//and the option to save to the same ini or save to a new ini file - and then let the user name the new save
	currentPresetPtr = std::make_shared<FBoidPreset>();

	// Presets["test"] = FBoidPreset();
	// Presets["test2"] = FBoidPreset();
	// Presets["test3"] = FBoidPreset();
	Save();
	
	Load();
}

void FImGuiModifier::Save()
{
	if (UBoidPresetSaves* PresetSavesInstance = Cast<UBoidPresetSaves>(UGameplayStatics::CreateSaveGameObject(UBoidPresetSaves::StaticClass())))
	{
		PresetSavesInstance->Presets = this->Presets;

		if (UGameplayStatics::SaveGameToSlot(PresetSavesInstance, SaveSlotName, 0))
		{
			UE_LOG(LogTemp, Log, TEXT("save successful"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("save failed"));
		}
	}
}

void FImGuiModifier::RenderImGui()
{
	
}

void FImGuiModifier::Load()
{
	if (UBoidPresetSaves* PresetLoadInstance = Cast<UBoidPresetSaves>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)))
	{
		UE_LOG(LogTemp, Log, TEXT("load successful"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("load failed"));
	}
}
