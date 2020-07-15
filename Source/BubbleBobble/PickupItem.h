// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

UCLASS()
class BUBBLEBOBBLE_API APickupItem : public AActor
{
	GENERATED_BODY()

	bool canFall{ true };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Score, meta = (AllowPrivateAccess = true))
	int scoringValue;

	//The list of sprites for the bookcase
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = true))
	TMap<FName, class UPaperSprite*> BookcaseSprites;

	//The list of sprites for the pickup items
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = true))
	TArray<class UPaperSprite*> itemSprites;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = true))
	class UBoxComponent* collisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = true))
	class UPaperSpriteComponent* spriteComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = true))
	class USoundBase* SFX;
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bookcase")
	FName BookcaseLetter;

	// Sets default values for this actor's properties
	APickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
