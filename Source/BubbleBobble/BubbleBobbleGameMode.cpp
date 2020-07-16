// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BubbleBobbleGameMode.h"
#include "BubbleBobbleCharacter.h"
#include "BubbleBobbleGameInstance.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

ABubbleBobbleGameMode::ABubbleBobbleGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = ABubbleBobbleCharacter::StaticClass();	
}

void ABubbleBobbleGameMode::BeginPlay()
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		World->GetTimerManager().SetTimer(loopTimeHandle, this, &ABubbleBobbleGameMode::UpdateTimer, 1.f, true);
	}
}

void ABubbleBobbleGameMode::Tick(float DeltaSeconds)
{
	
}

void ABubbleBobbleGameMode::UpdateTimer()
{
	static bool doOnce = true;
	Timer--;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Timer");
	if (Timer <= 0)
	{
		ABubbleBobbleCharacter* player = Cast<ABubbleBobbleCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		player->lives = 0;
	}
	else if (Timer <= 15)
	{
		if (doOnce)
		{
			UGameplayStatics::PlaySound2D(this, HurryUpSound);
			UBubbleBobbleGameInstance* gameInstance = Cast<UBubbleBobbleGameInstance>(UGameplayStatics::GetGameInstance(this));
			gameInstance->IncreasePitch();
			doOnce = false;
		}
	}
}