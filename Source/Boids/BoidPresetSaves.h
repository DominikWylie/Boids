// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "ImGuiModifier.h"

#include "BoidPresetSaves.generated.h"

/**
 * 
 */
UCLASS()
class BOIDS_API UBoidPresetSaves : public USaveGame
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere)
	TMap<FName, FBoidPreset> Presets;
	

	//plan: swap all the ini file stuff to this system it should be way more seemless
};
