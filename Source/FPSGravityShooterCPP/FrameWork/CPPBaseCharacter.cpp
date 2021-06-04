// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPBaseCharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "FPSGravityShooterCPP/UI/CPPMainInvetoryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "CustomMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACPPBaseCharacter::ACPPBaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer.SetDefaultSubobjectClass<UCustomMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACPPBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACPPBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACPPBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACPPBaseCharacter::RefreshInventory()
{
}

void ACPPBaseCharacter::StartMultiTrace()
{
	MultiSphreTrace();
}

void ACPPBaseCharacter::EndMultiTrace()
{
}

void ACPPBaseCharacter::PickupLineTrace()
{
	if (IsValid(CameraComponentRef))
	{
		LineTrace();
	}
	else
	{
		CameraComponentRef = Cast<UCameraComponent>(GetComponentByClass(UCameraComponent::StaticClass()));
		LineTrace();
	}
}

void ACPPBaseCharacter::LineTrace()
{
	const FVector start = CameraComponentRef->GetComponentLocation();
	const FVector cameraForwardVector = CameraComponentRef->GetForwardVector();
	const FVector end = start + (cameraForwardVector * 500.0f);

	FHitResult Hit;

	DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 10, 0, 1);

	const bool isHit = GetWorld()->LineTraceSingleByChannel(Hit, start, end, ECC_Visibility);
	if (isHit)
	{
		if (Hit.bBlockingHit)
		{
			/*
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, *Hit.GetActor()->GetName());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, FString::Printf(TEXT("Impact Point is:  &s"), *Hit.ImpactPoint.ToString()));
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, FString::Printf(TEXT("Impact normal is:  &s"), *Hit.ImpactNormal.ToString()));
			}
			*/

			IGameplayTagAssetInterface* GameplayTagsInterface = Cast<IGameplayTagAssetInterface>(Hit.Actor);
			if (GameplayTagsInterface)
			{
				if (GameplayTagsInterface->
					HasMatchingGameplayTag(ItemClassTag))
				{
					if (GameplayTagsInterface->HasMatchingGameplayTag(BoxCollection))
					{
						ABoxCollectionItem* castingItem = Cast<ABoxCollectionItem>(Hit.Actor);
						if (castingItem)
						{
							BoxItemRef = castingItem;
						}
						else
						{
							BoxItemRef = nullptr;
						}
					}
					else
					{
						AMasterItem* castingItem = Cast<AMasterItem>(Hit.Actor);
						if (castingItem)
						{
							ItemRef = castingItem;
						}
						else
						{
							ItemRef = nullptr;
						}
					}
				}
			}
		}
	}
}

void ACPPBaseCharacter::MultiSphreTrace()
{
	// create tarray for hit results
	TArray<FHitResult> OutHits;

	// start and end locations
	FVector SweepStart = GetActorLocation();
	FVector SweepEnd = GetActorLocation();

	// create a collision sphere
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(500.0f);

	// draw collision sphere
	DrawDebugSphere(GetWorld(), GetActorLocation(), MyColSphere.GetSphereRadius(), 50, FColor::Purple, true);

	// check if something got hit in the sweep
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic,
		MyColSphere);

	if (isHit)
	{
		// loop through TArray
		for (auto& Hit : OutHits)
		{
			IGameplayTagAssetInterface* GameplayTagsInterface = Cast<IGameplayTagAssetInterface>(Hit.Actor);
			if (GameplayTagsInterface)
			{
				if (GameplayTagsInterface->
					HasMatchingGameplayTag(ItemClassTag))
				{
					if (!(GameplayTagsInterface->HasMatchingGameplayTag(BoxCollection)))
					{
						AMasterItem* castingItem = Cast<AMasterItem>(Hit.Actor);
						if (castingItem && IsValid(castingItem))
						{
							MultiItemRef.Add(castingItem);
						}
					}
				}
			}
			if (GEngine)
			{
				// screen log information on what was hit
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
					FString::Printf(TEXT("Hit Result: %s"), *Hit.Actor->GetName()));
				// uncommnet to see more info on sweeped actor
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("All Hit Information: %s"), *Hit.ToString()));
			}
		}
	}
}

bool ACPPBaseCharacter::DestroyPickupItem_Validate(AMasterItem* itemRefParam)
{
	if (IsValid(itemRefParam))
	{
		return true;
	}

	return false;
}

void ACPPBaseCharacter::DestroyPickupItem_Implementation(AMasterItem* itemRefParam)
{
	itemRefParam->DestroyItem();
}

void ACPPBaseCharacter::AddToInventory()
{
	if (IsValid(ItemRef))
	{
		Inventory.Add(ItemRef->GetItemData());

		for (FItemData Data : Inventory)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Black, Data.Name.ToString());
		}

		DestroyPickupItem(ItemRef);
	}
}

