// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupItem.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "BubbleBobbleGameInstance.h"
#include "BubbleBobbleGameMode.h"
#include "BubbleBobbleCharacter.h"

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

	SetSprite();
}

void APickupItem::SetSprite()
{
	if (!BookcaseSprites.Num() || !itemSprites.Num())
	{
		return;
	}
	if (BookcaseLetter == "" && PowerUpType == EPowerUpType::eNone)
	{
		if (rand() % 10 != 0)
		{
			//NORMAL ITEM
			int numOfSprites = itemSprites.Num();
			int selectedSpriteIndex = rand() % numOfSprites - 1;
			spriteComp->SetSprite(itemSprites[selectedSpriteIndex]);
			scoringValue = 100;
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "item letter spawned");
		}
		else
		{
			//POWER UP
			int numOfSprites = powerUpSprites.Num();
			int selectedSpriteIndex = rand() % numOfSprites - 2;
			switch (selectedSpriteIndex)
			{
			case 0:
				//INVINCIBILITY
				spriteComp->SetSprite(powerUpSprites.FindRef(EPowerUpType::eInvicibility));
				PowerUpType = EPowerUpType::eInvicibility;
				break;
			case 1:
				//BUBBLEGUM
				spriteComp->SetSprite(powerUpSprites.FindRef(EPowerUpType::eBubbleGum));
				PowerUpType = EPowerUpType::eBubbleGum;
				break;
			case 2:
				//FEAR
				spriteComp->SetSprite(powerUpSprites.FindRef(EPowerUpType::eFear));
				PowerUpType = EPowerUpType::eFear;
				break;
			case 3:
				//CLOCK
				spriteComp->SetSprite(powerUpSprites.FindRef(EPowerUpType::eClock));
				PowerUpType = EPowerUpType::eClock;
				break;
			default:
				break;
			}
			scoringValue = 500;
		}
	}
	if(BookcaseLetter != "")
	{
		//BOOKCASE LETTER
		spriteComp->SetRelativeScale3D(FVector(0.5f));
		spriteComp->SetSprite(BookcaseSprites.FindRef(BookcaseLetter));
		scoringValue = 1000;
		canFall = false;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "bookcase letter spawned");
	}
	if (PowerUpType != EPowerUpType::eNone)
	{
		//POWER UP
		spriteComp->SetRelativeScale3D(FVector(0.5f));
		spriteComp->SetSprite(powerUpSprites.FindRef(PowerUpType));
		scoringValue = 500;
	}
}

void APickupItem::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->GetClass()->GetName().StartsWith("BP_2DSideScroller"))
	{
		if (!canFall)
		{
			UBubbleBobbleGameInstance* gameInstance = Cast<UBubbleBobbleGameInstance>(UGameplayStatics::GetGameInstance(this));
			gameInstance->score += scoringValue;
			if (PowerUpType == EPowerUpType::eNone)
			{
				UGameplayStatics::PlaySound2D(this, SFXs.FindRef("ItemSFX"));
			}	
			else
			{
				UGameplayStatics::PlaySound2D(this, SFXs.FindRef("PowerUpSFX"));
				if (PowerUpType == EPowerUpType::eInvicibility || PowerUpType == EPowerUpType::eBubbleGum)
				{
					ABubbleBobbleCharacter* player = Cast<ABubbleBobbleCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
					player->ActivatePowerUp(PowerUpType);
				}
				else if (PowerUpType == EPowerUpType::eFear || PowerUpType == EPowerUpType::eClock)
				{
					ABubbleBobbleGameMode* gameMode = Cast<ABubbleBobbleGameMode>(UGameplayStatics::GetGameMode(this));
					gameMode->ActivatePowerUp(PowerUpType);
				}
			}			
			if(BookcaseLetter != "")
			{
				gameInstance->lettersCollected.Add(BookcaseLetter.ToString());
				if (gameInstance->lettersCollected.Num() == 8)
				{
					UGameplayStatics::OpenLevel(this, "Level_Bookcase");
				}				
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "bookcase letter item collected");
			}
			Destroy();
		}		
	}
	else if ((OtherActor->GetClass()->GetName().Contains("Platform") || OtherActor->GetClass()->GetName().Contains("Wall")) && BookcaseLetter == "")
	{
		canFall = false;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "item landed on the platform");
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
		if (PowerUpType == EPowerUpType::eFear)
		{
			ABubbleBobbleCharacter* temp = Cast<ABubbleBobbleCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			UKismetSystemLibrary::MoveComponentTo(RootComponent, temp->GetActorLocation(), FRotator(0.0f, 0.0f, 0.0f), false, true, 0.4f, false, EMoveComponentAction::Move, FLatentActionInfo());
		}
	}
	else
	{
		if (doOnce)
		{
			SetSprite();
			doOnce = false;
		}
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

