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

enum class EPowerUpType : uint8;

UCLASS(minimalapi)
class ABubbleBobbleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABubbleBobbleGameMode();

	void ActivatePowerUp(EPowerUpType powerUp) noexcept;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool FearActivated{ false };
	void DeactivateFear() noexcept { FearActivated = false; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool SlowDownActivated{ false };
	void DeactivateSlowDown() noexcept { SlowDownActivated = false; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int Timer{ 120 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* HurryUpSound;

private:
	FTimerHandle loopTimeHandle;
	FTimerHandle timerHandler;
	virtual void BeginPlay() override;
	void UpdateTimer();
};
