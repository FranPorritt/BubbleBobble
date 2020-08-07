// Fill out your copyright notice in the Description page of Project Settings.

#include "Platform_BP2_Electric_Boogaloo.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "BubbleBobbleCharacter.h"

// Sets default values
APlatform_BP2_Electric_Boogaloo::APlatform_BP2_Electric_Boogaloo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	

	sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Paper Sprite"));
	sprite->SetRelativeScale3D(FVector(1.0f, 4.0f, 1.0f));
	sprite->SetMobility(EComponentMobility::Static);
	RootComponent = sprite;

	colBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	colBox->SetRelativeLocation(FVector(0, 0, 280));
	colBox->SetRelativeScale3D(FVector(4.25, 1.0, 0.5));
	colBox->OnComponentBeginOverlap.AddDynamic(this, &APlatform_BP2_Electric_Boogaloo::OnOverlapBegin);
	colBox->OnComponentEndOverlap.AddDynamic(this, &APlatform_BP2_Electric_Boogaloo::OnOverlapEnd);
	colBox->SetupAttachment(sprite);

}

// Called when the game starts or when spawned
void APlatform_BP2_Electric_Boogaloo::BeginPlay()
{
	Super::BeginPlay();

	SetSprite();
}

void APlatform_BP2_Electric_Boogaloo::Activate_Floor_Player()
{
	sprite->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

void APlatform_BP2_Electric_Boogaloo::Activate_Floor_Enemy()
{
	sprite->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
}

void APlatform_BP2_Electric_Boogaloo::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->GetClass()->GetName().Contains("BP_2DSideScroller"))
	{
		if (colBox->GetComponentLocation().Z < OtherActor->GetActorLocation().Z) // Coming from above
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Player has collided with the platform FROM ABOVE");
			sprite->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Player has collided with the platform FROM BELLOW");			
			GetWorldTimerManager().SetTimer(loopTimeHandle, this, &APlatform_BP2_Electric_Boogaloo::Activate_Floor_Player, 0.2f, false);					
		}
	}
	else
	{
		if (OtherActor->GetClass()->GetName().Contains("Enemy"))
		{
			if (colBox->GetComponentLocation().Z < OtherActor->GetActorLocation().Z)
			{
				sprite->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
			}
			else
			{
				GetWorldTimerManager().SetTimer(loopTimeHandle, this, &APlatform_BP2_Electric_Boogaloo::Activate_Floor_Enemy, 0.2f, false);				
			}
		}
	}
}

void APlatform_BP2_Electric_Boogaloo::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->GetName().Contains("BP_2DSideScroller"))
	{
		sprite->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
	else if (OtherActor->GetClass()->GetName().Contains("Enemy"))
	{
		sprite->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	}
}

// Called every frame
void APlatform_BP2_Electric_Boogaloo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//if platform is higher than the player then the platform will ignore so that the player can go through
	ABubbleBobbleCharacter* tempPlayer = Cast<ABubbleBobbleCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (colBox->GetComponentLocation().Z > (tempPlayer->GetActorLocation().Z + 100.0f))
	{
		if (sprite->GetCollisionResponseToChannel(ECC_Pawn) != ECR_Ignore)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Platform higher than the player");
			sprite->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		}
	}
}

void APlatform_BP2_Electric_Boogaloo::SetSprite()
{
	sprite->SetSprite(tileSprites.FindRef(desiredSprite));
	sprite->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	sprite->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	sprite->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Block);
}
