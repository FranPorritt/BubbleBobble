// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupItem.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickupItem::APickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	spriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComp"));
	RootComponent = spriteComp;

	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	collisionBox->SetRelativeScale3D(FVector(4.0f, 1.0f, 4.0f));
	collisionBox->OnComponentBeginOverlap.AddDynamic(this, &APickupItem::OnOverlapBegin);
	collisionBox->SetupAttachment(spriteComp);
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();

	if (!BookcaseSprites.Num() || !itemSprites.Num())
	{
		return;
	}
	if (BookcaseLetter == "")
	{
		int numOfSprites = itemSprites.Num();
		int selectedSpriteIndex = rand() % numOfSprites - 1;
		spriteComp->SetSprite(itemSprites[selectedSpriteIndex]);
		scoringValue = 100;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "item letter spawned");
	}
	else
	{
		spriteComp->SetRelativeScale3D(FVector(0.5f));
		spriteComp->SetSprite(BookcaseSprites.FindRef(BookcaseLetter));
		scoringValue = 1000;
		canFall = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "bookcase letter spawned");
	}	
}

void APickupItem::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->GetClass()->GetName().StartsWith("BP_2DSideScroller"))
	{
		//UGameplayStatics::GetGameInstance();
		//add score to the game instance
		
		UGameplayStatics::PlaySound2D(this, SFX);
		if (BookcaseLetter == "")
		{
			Destroy();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "normal item collected");
		}
		else
		{
			//add letter collected to the game instance
			//check if all letters have been collected 
			Destroy();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "bookcase letter item collected");
		}
	}
	else if (OtherActor->GetClass()->GetName().Contains("Platform") || OtherActor->GetClass()->GetName().Contains("Wall"))
	{
		canFall = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "item landed on the platform");
	}
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BookcaseLetter == "")
	{
		if (canFall)
		{
			//make the item fall
			FVector location = GetActorLocation();
			location -= FVector(0.0f, 0.0f, 4.0f);
			SetActorLocation(location);
		}
	}
	else
	{
		//make the letter float
		FVector location = GetActorLocation();
		location += FVector(0.0f, 0.0f, 1.5f);
		SetActorLocation(location);
		if (GetActorLocation().Z >= 2600.0f)
		{
			Destroy();
		}
	}
}

