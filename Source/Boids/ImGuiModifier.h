#pragma once

#include <string>

struct ImGuiModifier
{
	//keep the default values above for safety 
	//even tho they are replaces with the saved ones

	float CenteringFactor = 3.3f;
	float MatchingFactor = 4.5f;
	float AvoidBoidsFactor = 4.7f;
	float GeneralTurningSpeed = 0.6f;
	float Speed = 2507.f;
	float ProtectedRange = 250.f;
	float VisualRange = 1160.f;

	ImGuiModifier();

	void Save();

	void Load();

private:
	bool Initialised = false;

	std::string directory = "Content/ImGuiTuningValues.ini";
};

