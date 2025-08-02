#include "ImGuiModifier.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

ImGuiModifier::ImGuiModifier()
{
    if (!Initialised) {
        Load();
        Initialised = true;
    }
}

void ImGuiModifier::Save()
{
    //if (GEngine)
    //    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("current directory %s"), UTF8_TO_TCHAR(std::filesystem::current_path().string().c_str())));

    FString Path = FPaths::ProjectDir() / directory.c_str();

    std::ofstream file(TCHAR_TO_UTF8(*Path));
    if (!file) return;

    file << "[BoidSettings]\n";
    file << "CenteringFactor=" << CenteringFactor << "\n";
    file << "MatchingFactor=" << MatchingFactor << "\n";
    file << "AvoidBoidsFactor=" << AvoidBoidsFactor << "\n";
    file << "GeneralTurningSpeed=" << GeneralTurningSpeed << "\n";
    file << "Speed=" << Speed << "\n";
    file << "ProtectedRange=" << ProtectedRange << "\n";
    file << "VisualRange=" << VisualRange << "\n";
    file << "BoidMax=" << BoidMax << "\n";

    file.close();
}

void ImGuiModifier::Load()
{
    FString Path = FPaths::ProjectDir() / directory.c_str();

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

    file.close();
}
