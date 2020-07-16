// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BubbleBobbleGameMode.generated.h"

/**
 * The GameMode defines the game being played. It governs the game rules, scoring, what actors
 * are allowed to exist in this game type, and who may enter the game.
 *
 * This game mode just sets the default pawn to be the MyCharacter asset, which is a subclass of BubbleBobbleCharacter
 */
UCLASS(minimalapi)
class ABubbleBobbleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABubbleBobbleGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool FearActivated{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int Timer{ 120 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* HurryUpSound;

private:
	FTimerHandle loopTimeHandle;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void UpdateTimer();
};
