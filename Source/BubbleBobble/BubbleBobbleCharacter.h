// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "BubbleBobbleCharacter.generated.h"

/**
 * This class is the default character for BubbleBobble, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */

enum class EPowerUpType : uint8;

UENUM(BlueprintType)
enum class EAnimationStates : uint8
{
	eIdle,
	eRunning,
	eFiring,
	eJumping
};

UENUM(BlueprintType)
enum class EPlayerPower : uint8
{
	eStandard,
	eBubblegum
};

UCLASS(config=Game)
class ABubbleBobbleCharacter : public APaperCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	EAnimationStates AnimationState = EAnimationStates::eIdle;
	EAnimationStates DesiredAnimation;

	/* Cooldowns */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float shootingCooldown{ 0.5f };
	bool canShoot{ true };
	void CanShootAgain() noexcept { canShoot = true; DesiredAnimation = EAnimationStates::eIdle; }

	FTimerHandle animationsTimeHandler;
	FTimerHandle powerUpTimeHandler;
	FTimerHandle fireTimeHandler;

	//UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	void StopAnimation(float argWaitingTime);
	virtual void Jump() override;
	virtual void StopJumping() override;

	void SetImmunity(bool state) { isImmune = state; }
	void DeactivateImmunity() { isImmune = false; }
	void DeactivateBubbleGum() { currentPower = EPlayerPower::eStandard; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector spawnPos;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool isImmune = false;

	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	// The animation to play while firing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* FiringAnimation;

	// The animation to play while jumping
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* JumpingAnimation;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* FireSound;

	/** Sound to play each time we jump */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* JumpSound;

	/** Sound to play each time we jump */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds)
	class USoundBase* RespawnSound;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);

	void UpdateCharacter();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Shooting 
	UFUNCTION(BlueprintCallable, Category = "My Functions")
	void Fire();


public:
	ABubbleBobbleCharacter();

	UFUNCTION(BlueprintCallable)
	void Respawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	EPlayerPower currentPower = EPlayerPower::eStandard;

	UFUNCTION()
	void onTimerEnd();

	void ActivatePowerUp(EPowerUpType powerUp) noexcept;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int lives = 3;

	// Projectile class to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class ABubble> BubbleBPClass;
};
