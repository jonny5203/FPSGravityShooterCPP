// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPBaseCharacter.h"
#include "CPPPlayerController.h"
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

	PickupLineTrace();
}

// Called to bind functionality to input
void ACPPBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACPPBaseCharacter::RefreshInventoryInterface()
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

	//DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 10, 0, 1);

	const bool isHit = GetWorld()->LineTraceSingleByChannel(Hit, start, end, ECC_GameTraceChannel2);
	if (isHit)
	{
		if (Hit.bBlockingHit)
		{
			if (IsValid(Hit.GetActor()))
			{
				if (ItemRef != Hit.GetActor())
				{
					IGameplayTagAssetInterface* GameplayTagsInterface = Cast<IGameplayTagAssetInterface>(Hit.Actor);
					if (GameplayTagsInterface)
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
								ItemRef->EnableWidgetVisibility(this);

								if (IsValid(ItemRef))
								{
									if (HasAuthority())
									{
										if (GEngine)
										{
											GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black,
											                                 " testing");
										}
										ServerItemAddPawn(ItemRef, Controller);
									}
								}
							}
							else
							{
								ItemRef = nullptr;
							}
						}
					}
				}
			}
			else
			{
				if (IsValid(ItemRef))
				{
					ItemRef->DisableWidgetVisibility(this);
					ServerItemRemovePawn(ItemRef);
					ItemRef = nullptr;
				}
			}
		}
	}
	else
	{
		if (IsValid(ItemRef))
		{
			ItemRef->DisableWidgetVisibility(this);
			//ServerItemRemovePawn(ItemRef);
			ItemRef = nullptr;
		}
	}
}

void ACPPBaseCharacter::MultiSphreTrace()
{
	MultiItemRef.Empty();
	
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
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_GameTraceChannel2,
	                                             MyColSphere);

	if (isHit)
	{
		// loop through TArray
		for (auto& Hit : OutHits)
		{
			IGameplayTagAssetInterface* GameplayTagsInterface = Cast<IGameplayTagAssetInterface>(Hit.Actor);
			if (GameplayTagsInterface)
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

bool ACPPBaseCharacter::ServerItemAddPawn_Validate(AMasterItem* itemRefParam, AController* PCRefParam)
{
	if (IsValid(itemRefParam) && IsValid(PCRefParam))
	{
		return true;
	}
	return false;
}

void ACPPBaseCharacter::ServerItemAddPawn_Implementation(AMasterItem* itemRefParam, AController* PCRefParam)
{
	itemRefParam->ServerAddPawnRef(Controller);
}

bool ACPPBaseCharacter::ServerItemRemovePawn_Validate(AMasterItem* itemRefParam)
{
	if (IsValid(itemRefParam))
	{
		return true;
	}
	return false;
}

void ACPPBaseCharacter::ServerItemRemovePawn_Implementation(AMasterItem* itemRefParam)
{
	itemRefParam->ServerRemovePawnRef(Controller);
}

void ACPPBaseCharacter::AddItemToInventory(const FItemData& ItemDataParam)
{
	Inventory.Add(ItemDataParam);
}

void ACPPBaseCharacter::DestroyMasterItem(AMasterItem* MasterItemParam)
{
	ServerDestroyMasterItem(MasterItemParam);
}

bool ACPPBaseCharacter::ServerDestroyMasterItem_Validate(AMasterItem* MasterItemParam)
{
	if (IsValid(MasterItemParam))
	{
		return true;
	}
	return false;
}

void ACPPBaseCharacter::ServerDestroyMasterItem_Implementation(AMasterItem* MasterItemParam)
{
	MasterItemParam->ServerDestroyItem();
}

void ACPPBaseCharacter::RebuildInventoryWidget()
{
	
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
