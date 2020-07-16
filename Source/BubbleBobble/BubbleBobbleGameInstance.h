// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BubbleBobbleGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BUBBLEBOBBLE_API UBubbleBobbleGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
	int lives{ 3 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
	int score;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
	TArray<FString> lettersCollected;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio)
	class UAudioComponent* backgroundMusic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio)
	class USoundBase* normalMusic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio)
	class USoundBase* fearMusic;

	UFUNCTION()
	void SetBackgroundMusic();

	UFUNCTION()
	void IncreasePitch();
	
};
