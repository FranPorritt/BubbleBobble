// Fill out your copyright notice in the Description page of Project Settings.

#include "BubbleBobbleGameInstance.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

void UBubbleBobbleGameInstance::SetBackgroundMusic()
{
	if (normalMusic == nullptr || fearMusic == nullptr)
	{
		return;
	}
	if (backgroundMusic == nullptr)
	{
		backgroundMusic = UGameplayStatics::SpawnSound2D(this, normalMusic, 1.0f, 1.0f, 0.0f, nullptr, true, true);
		return;
	}
	if (backgroundMusic->IsPlaying())
	{
		if (backgroundMusic->PitchMultiplier > 1.0f)
		{
			backgroundMusic->SetPitchMultiplier(1.0f);
		}
	}
	FString levelName = UGameplayStatics::GetCurrentLevelName(this);
	if (levelName == "Level_Bookcase" || levelName == "Level_1" || levelName == "MainMenu")
	{
		if (backgroundMusic->Sound != normalMusic)
		{
			backgroundMusic->Stop();
			backgroundMusic = UGameplayStatics::SpawnSound2D(this, normalMusic, 1.0f, 1.0f, 0.0f, nullptr, true, true);
		}
	}
	else
	{
		if (backgroundMusic->Sound != fearMusic)
		{
			backgroundMusic->Stop();
			backgroundMusic = UGameplayStatics::SpawnSound2D(this, fearMusic, 1.0f, 1.0f, 0.0f, nullptr, true, true);
		}
	}
}

void UBubbleBobbleGameInstance::IncreasePitch()
{
	backgroundMusic->SetPitchMultiplier(2.0f);
}
