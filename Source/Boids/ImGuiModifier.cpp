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
	
	Load();
	
	*currentPresetPtr = Presets[NamesArr[CurrentItem]];
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
	
	if (ImGui::Begin("boids controller (press P to edit)", nullptr, ImGuiWindowFlags_MenuBar)) {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::Button("Back to game")) {
				FImGuiModule::Get().GetProperties().ToggleInput();
				ImGui::SetWindowCollapsed(true);
				PC->SetInputMode(FInputModeGameOnly());
			}

			if (ImGui::Button("Save All")) {
				Save();
			}

			if (ImGui::Button("Load")) {
				Load();
			}

			if (ImGui::Button("Make new Preset")) {
				FString NewPreset = "New Preset";
				while (NamesArr.Contains(NewPreset))
				{
					NewPreset = NewPreset + " New";
					if(GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("nameloop"));
				}
				FName CompletePresetName = FName(*NewPreset);
				if (!Presets.Contains(CompletePresetName))
				{
					//ideally the user can edit one and save ut as another but i have made it so you can only save the
					//whole thing and when you change values it updates the data on the specific preset automatically.
					//the user should still be able to recover old presets though version control.
					Presets.Add(CompletePresetName, Presets[NamesArr[CurrentItem]]);
					NamesArr.Add(CompletePresetName);
				}else
				{
					if(GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("preset contained that"));
				}
				
			}
			
			if (ImGui::BeginCombo("Presets", TCHAR_TO_UTF8(*NamesArr[CurrentItem].ToString()), ImGuiComboFlags_None))
			{
				for (int i = 0; i < NamesArr.Num(); i++)
				{
					bool IsSelected = (CurrentItem == i);
					if (ImGui::Selectable(TCHAR_TO_UTF8(*NamesArr[i].ToString()), IsSelected))
					{
						//Presets[NamesArr[CurrentItem]] = *currentPresetPtr;
						
						CurrentItem = i;

						*currentPresetPtr = Presets[NamesArr[CurrentItem]];
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

			ImGui::EndMenuBar();
		}
		//			ImGui::EndMenuBar();

		//ImGui::Text(std::to_string(Octree->GetNodeNum()).c_str());

		const float totalWidth = ImGui::GetContentRegionAvail().x;  // total available width
		//const float labelWidth = ImGui::CalcTextSize("Change Name").x; // width of the label
		ImGui::PushItemWidth(totalWidth - 180.0f); // leave a small margin

		bool edited = false;
		if (
		ImGui::SliderFloat("Centering factor", &(currentPresetPtr->CenteringFactor), 0.f, 10.f) ||
		ImGui::SliderFloat("Matching factor", &(currentPresetPtr->MatchingFactor), 0.f, 100.f) ||
		ImGui::SliderFloat("Avoidance factor", &(currentPresetPtr->AvoidBoidsFactor), 0.f, 10.f) ||
		ImGui::SliderFloat("General turning speed", &(currentPresetPtr->GeneralTurningSpeed), 0.f, 10.f) ||
		ImGui::SliderFloat("Boids speed", &(currentPresetPtr->Speed), 0.f, 10000.f) ||
		
		ImGui::SliderFloat("Visual range", &(currentPresetPtr->VisualRange), 0.f, 10000.f) ||
		ImGui::SliderFloat("Protected range", &(currentPresetPtr->ProtectedRange), 0.f, 7000.f))
		{
			edited = true;
		}
		
		if (ImGui::SliderInt("Boid max", &(currentPresetPtr->BoidMax), 0.f, 1000.f)) {
			Octree->ReduceNodes(currentPresetPtr->BoidMax);
			edited = true;
		}

		if (edited)
		{
			Presets[NamesArr[CurrentItem]] = *currentPresetPtr;
		}

		static char NameInputBuffer[128] = "";

		ImGui::InputText("Change Name", NameInputBuffer, IM_ARRAYSIZE((NameInputBuffer)));

		ImGui::SameLine();

		if (ImGui::Button("Update Name"))
		{
			if (Presets.Contains(NameInputBuffer))
			{
				//exists, do popup saying cant name it something that already exists
			}else
			{
				FBoidPreset presetToMove;
				Presets.RemoveAndCopyValue(NamesArr[CurrentItem], presetToMove);
				NamesArr[CurrentItem] = NameInputBuffer;
				Presets.Add(NamesArr[CurrentItem], presetToMove);

				*currentPresetPtr = Presets[NamesArr[CurrentItem]];
			}
		}

		ImGui::PopItemWidth();
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
		this->Presets = PresetLoadInstance->Presets;
		Presets.GetKeys(NamesArr);
		
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("load successful"));
	}
	else
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("load failed"));
	}
}
