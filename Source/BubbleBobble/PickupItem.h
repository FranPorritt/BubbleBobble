// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

UENUM(BlueprintType)
enum class EPowerUpType : uint8
{
	eInvicibility,
	eBubbleGum,
	eFear,
	eClock,
	eNone,
};

UCLASS()
class BUBBLEBOBBLE_API APickupItem : public AActor
{
	GENERATED_BODY()

	bool canFall{ true };
	bool doOnce{ true };
	FTimerHandle timerHandler;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Score, meta = (AllowPrivateAccess = true))
	int scoringValue;

	//The list of sprites for the bookcase
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = true))
	TMap<FName, class UPaperSprite*> BookcaseSprites;

	//The list of sprites for the pickup items
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = true))
	TArray<class UPaperSprite*> itemSprites;

	//The list of sprites for the power ups
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = true))
	TMap<EPowerUpType, class UPaperSprite*> powerUpSprites;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = true))
	TMap<FName, class USoundBase*> SFXs;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = true))
	class UBoxComponent* collisionBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = true))
	class UPaperSpriteComponent* spriteComp;
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bookcase")
	FName BookcaseLetter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PowerUp")
	EPowerUpType PowerUpType { EPowerUpType::eNone };

	// Sets default values for this actor's properties
	APickupItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SetSprite();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
