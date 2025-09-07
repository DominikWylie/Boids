#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

struct BoidPreset {
	//keep the default values above for safety 
//even tho they are replaces with the saved ones

	float CenteringFactor = 3.3f;
	float MatchingFactor = 4.5f;
	float AvoidBoidsFactor = 4.7f;
	float GeneralTurningSpeed = 0.6f;
	float Speed = 2507.f;
	float ProtectedRange = 250.f;
	float VisualRange = 1160.f;
	int32 BoidMax = 200;
};

struct ImGuiModifier
{
	ImGuiModifier();

	void Save();

	void Load();

	//set and forget, this pointer changes when presets are swapped
	const std::shared_ptr<const std::shared_ptr<const BoidPreset>> GetCurrentPreset() const;

	const std::vector<std::string>& GetPresetNames() const{ return presetNames; }
	void SetCurrentPreset(std::string newPreset);

	void NewLoadTemp();

private:
	bool Initialised = false;

	std::string directory = "Content/Boids/ValuePresets";

	std::map<std::string, BoidPreset> presets;

	std::vector<std::string> presetNames;

	std::shared_ptr<std::shared_ptr<BoidPreset>> currentPresetPtr;
};

