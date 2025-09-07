#include "ImGuiModifier.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

ImGuiModifier::ImGuiModifier()
{
	if (!Initialised) {
		try {
			for (const auto& entry : std::filesystem::directory_iterator(directory)) {
				presetNames.push_back(entry.path().filename().string());
			}
		}
		catch (const std::filesystem::filesystem_error& e) {

			FString ErrorMsg = UTF8_TO_TCHAR(e.what());

			UE_LOG(LogTemp, Error, TEXT("Filesystem error: %s [%s:%d] in %s"),
				*ErrorMsg,
				TEXT(__FILE__),
				__LINE__,
				TEXT(__FUNCTION__));
		}

		//load all data

		presets["default"];

		//this is a copy of the object - whech keeps the code safe, ill need to decide how to handle memory of modified vars
		//maybe save them to the map and add a note to the imgui window mentioning unsaved changes,
		//and the option to save to the same ini or save to a new ini file - and then let the user name the new save
		currentPresetPtr = std::make_shared<std::shared_ptr<BoidPreset>>(std::make_shared<BoidPreset>(presets["default"]));

		//Load();
		//Initialised = true;
	}
}

void ImGuiModifier::Save()
{
	//if (GEngine)
	//    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("current directory %s"), UTF8_TO_TCHAR(std::filesystem::current_path().string().c_str())));

	//FString Path = FPaths::ProjectDir() / directory.c_str();

	//std::ofstream file(TCHAR_TO_UTF8(*Path));
	//if (!file) return;

	//file << "[BoidSettings]\n";
	//file << "CenteringFactor=" << CenteringFactor << "\n";
	//file << "MatchingFactor=" << MatchingFactor << "\n";
	//file << "AvoidBoidsFactor=" << AvoidBoidsFactor << "\n";
	//file << "GeneralTurningSpeed=" << GeneralTurningSpeed << "\n";
	//file << "Speed=" << Speed << "\n";
	//file << "ProtectedRange=" << ProtectedRange << "\n";
	//file << "VisualRange=" << VisualRange << "\n";
	//file << "BoidMax=" << BoidMax << "\n";

	//file.close();
}

void ImGuiModifier::SetCurrentPreset(std::string newPreset)
{
	if (presets.find(newPreset) != presets.end()) {
		currentPreset = &presets[newPreset];
	}
}

void ImGuiModifier::NewLoadTemp() {
	//for (std::string& presetName : presetsName) {
	//	FString Path = FPaths::ProjectDir() / directory.c_str() / presetName.c_str();

	//	std::ifstream file(TCHAR_TO_UTF8(*Path));
	//	if (!file) return;

	//	std::string line;
	//	while (std::getline(file, line)) {
	//		std::istringstream ss(line);
	//		std::string key;
	//		if (std::getline(ss, key, '=')) {
	//			std::string valueStr;
	//			if (std::getline(ss, valueStr)) {
	//				float value = std::stof(valueStr);

	//				if (key == "CenteringFactor")			presets[presetName].CenteringFactor = value;
	//				else if (key == "MatchingFactor")		presets[presetName].MatchingFactor = value;
	//				else if (key == "AvoidBoidsFactor")		presets[presetName].AvoidBoidsFactor = value;
	//				else if (key == "GeneralTurningSpeed")	presets[presetName].GeneralTurningSpeed = value;
	//				else if (key == "Speed")				presets[presetName].Speed = value;
	//				else if (key == "ProtectedRange")		presets[presetName].ProtectedRange = value;
	//				else if (key == "VisualRange")			presets[presetName].VisualRange = value;
	//				else if (key == "BoidMax")				presets[presetName].BoidMax = value;

	//			}
	//		}
	//	}

	//	file.close();
	//}

	//int temp = 1234;
}

void ImGuiModifier::Load()
{
	for (std::string& presetName : presetNames) {
		FString Path = FPaths::ProjectDir() / directory.c_str() / presetName.c_str();

		std::ifstream file(TCHAR_TO_UTF8(*Path));
		if (!file) return;

		std::string line;
		while (std::getline(file, line)) {
			std::istringstream ss(line);
			std::string key;
			if (std::getline(ss, key, '=')) {
				std::string valueStr;
				if (std::getline(ss, valueStr)) {
					float value = std::stof(valueStr);

					if (key == "CenteringFactor")			presets[presetName].CenteringFactor = value;
					else if (key == "MatchingFactor")		presets[presetName].MatchingFactor = value;
					else if (key == "AvoidBoidsFactor")		presets[presetName].AvoidBoidsFactor = value;
					else if (key == "GeneralTurningSpeed")	presets[presetName].GeneralTurningSpeed = value;
					else if (key == "Speed")				presets[presetName].Speed = value;
					else if (key == "ProtectedRange")		presets[presetName].ProtectedRange = value;
					else if (key == "VisualRange")			presets[presetName].VisualRange = value;
					else if (key == "BoidMax")				presets[presetName].BoidMax = value;

				}
			}
		}

		file.close();
		
		if (!currentPreset) {
			currentPreset = &presets[presetName];
		}
	}

	int temp = 1234;

	/*FString Path = FPaths::ProjectDir() / directory.c_str();

	std::ifstream file(TCHAR_TO_UTF8(*Path));
	if (!file) return;

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream ss(line);
		std::string key;
		if (std::getline(ss, key, '=')) {
			std::string valueStr;
			if (std::getline(ss, valueStr)) {
				float value = std::stof(valueStr);

				if (key == "CenteringFactor") CenteringFactor = value;
				else if (key == "MatchingFactor") MatchingFactor = value;
				else if (key == "AvoidBoidsFactor") AvoidBoidsFactor = value;
				else if (key == "GeneralTurningSpeed") GeneralTurningSpeed = value;
				else if (key == "Speed") Speed = value;
				else if (key == "ProtectedRange") ProtectedRange = value;
				else if (key == "VisualRange") VisualRange = value;
				else if (key == "BoidMax") BoidMax = value;

			}
		}
	}

	file.close();*/
}

const std::shared_ptr<std::shared_ptr<BoidPreset>> ImGuiModifier::GetCurrentPreset()
{
	return const std::shared_ptr<BoidPreset>(currentPreset);
}
