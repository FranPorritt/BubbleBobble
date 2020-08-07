// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Platform_BP2_Electric_Boogaloo.generated.h"

UCLASS()
class BUBBLEBOBBLE_API APlatform_BP2_Electric_Boogaloo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlatform_BP2_Electric_Boogaloo();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite)
	int desiredSprite;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void Activate_Floor_Player();

	UFUNCTION()
	void Activate_Floor_Enemy();	

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = true))
	class UPaperSpriteComponent* sprite;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = true))
	TMap<int, class UPaperSprite*> tileSprites;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = true))
	class UBoxComponent* colBox;

	FTimerHandle loopTimeHandle;

	void SetSprite();

};
