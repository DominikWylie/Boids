#include "ImGuiModifier.h"

// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <filesystem>

#include "imgui.h"
#include "ImGuiInputHandler.h"
#include "ImGuiModule.h"
#include "OctreeMain.h"
#include <string>

#include "BoidPresetSaves.h"
#include "Kismet/GameplayStatics.h"

FImGuiModifier::FImGuiModifier()
{
	//load all data

	//this is a copy of the object - whech keeps the code safe, ill need to decide how to handle memory of modified vars
	//maybe save them to the map and add a note to the imgui window mentioning unsaved changes,
	//and the option to save to the same ini or save to a new ini file - and then let the user name the new save
	currentPresetPtr = std::make_shared<FBoidPreset>();
}

void FImGuiModifier::BeginPlay(UWorld* World)
{
	PC = World->GetFirstPlayerController();

	Octree = Cast<AOctreeMain>(UGameplayStatics::GetActorOfClass(World, AOctreeMain::StaticClass()));

	Presets.Add("test", FBoidPreset());
	Presets.Add("test2", FBoidPreset());
	Presets.Add("test3", FBoidPreset());

	Presets["test"].BoidMax = 175;
	Presets["test2"].BoidMax = 150;
	Presets["test3"].BoidMax = 125;
	
	Save();
	Load();
}

void  FImGuiModifier::RenderImGui()
	{
	if (PC->WasInputKeyJustPressed("P")) {
		FImGuiModule::Get().GetProperties().ToggleInput();
		ImGui::SetNextWindowCollapsed(false);
	}

	ImGui::SetNextWindowBgAlpha(0.3f);

	// if (ShowVisualRangeSpheres) {
	// 	DrawDebugSphere(GetWorld(), CentreBounds, ImGuiMods.ProtectedRange, 10, FColor::Red);
	// 	DrawDebugSphere(GetWorld(), CentreBounds, ImGuiMods.VisualRange, 10, FColor::Green);
	// }

	if (ImGui::Begin("When will you wear wigs", nullptr, ImGuiWindowFlags_MenuBar)) {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::Button("Back to game")) {
				FImGuiModule::Get().GetProperties().ToggleInput();
				ImGui::SetWindowCollapsed(true);
				PC->SetInputMode(FInputModeGameOnly());
			}

			if (ImGui::Button("Save")) {
				Save();
			}

			if (ImGui::Button("Load")) {
				Load();
			}
			TArray<FName> NamesArr;
			Presets.GetKeys(NamesArr);
			
			TArray<const char*> PresetNamesConstSt;
			
			for (const FName &Name: NamesArr)
			{
				PresetNamesConstSt.Add(TCHAR_TO_UTF8(*Name.ToString()));
			}

			static int32 CurrentItem = 0;
			//PresetNamesConstSt[CurrentItem]
			if (ImGui::BeginCombo("Presets", TCHAR_TO_UTF8(*NamesArr[CurrentItem].ToString())))
			{
				for (int i = 0; i < PresetNamesConstSt.Num(); i++)
				{
					bool IsSelected = (CurrentItem == i);
					if (ImGui::Selectable(TCHAR_TO_UTF8(*NamesArr[i].ToString()), IsSelected))
					{
						CurrentItem = i;
					}
					if (IsSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			// if (ImGui::Button("Toggle visual range")) {
			// 	ShowVisualRangeSpheres = !ShowVisualRangeSpheres;
			// }
		}
		ImGui::EndMenuBar();
		
		//ImGui::Text(std::to_string(Octree->GetNodeNum()).c_str());
		
		ImGui::SliderFloat("Centering factor", &(currentPresetPtr->CenteringFactor), 0.f, 10.f);
		ImGui::SliderFloat("Matching factor", &(currentPresetPtr->MatchingFactor), 0.f, 100.f);
		ImGui::SliderFloat("Avoidance factor", &(currentPresetPtr->AvoidBoidsFactor), 0.f, 10.f);
		ImGui::SliderFloat("General turning speed", &(currentPresetPtr->GeneralTurningSpeed), 0.f, 10.f);
		ImGui::SliderFloat("Boids speed", &(currentPresetPtr->Speed), 0.f, 10000.f);
		
		ImGui::SliderFloat("Visual range", &(currentPresetPtr->VisualRange), 0.f, 10000.f);
		ImGui::SliderFloat("Protected range", &(currentPresetPtr->ProtectedRange), 0.f, 7000.f);
		
		if (ImGui::SliderInt("Boid max", &(currentPresetPtr->BoidMax), 0.f, 1000.f)) {
			Octree->ReduceNodes(currentPresetPtr->BoidMax);
		}
	}


	ImGui::End();
}

void FImGuiModifier::Save()
{
	if (UBoidPresetSaves* PresetSavesInstance = Cast<UBoidPresetSaves>(UGameplayStatics::CreateSaveGameObject(UBoidPresetSaves::StaticClass())))
	{
		PresetSavesInstance->Presets = this->Presets;

		if (UGameplayStatics::SaveGameToSlot(PresetSavesInstance, SaveSlotName, 0))
		{
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("save successful"));
		}
		else
		{
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("save failed"));
		}
	}
}

void FImGuiModifier::Load()
{
	if (UBoidPresetSaves* PresetLoadInstance = Cast<UBoidPresetSaves>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)))
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("load successful"));
	}
	else
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("load failed"));
	}
}
