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

	PCInterfaceRef = nullptr;
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

	if (GetCharacterMovement()->Velocity.Size() != 0)
	{
		MultiSphereTrace();
	}

	PickupLineTrace();
}

// Called to bind functionality to input
void ACPPBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACPPBaseCharacter::AddToInventory);
}

void ACPPBaseCharacter::StartMultiTrace()
{
	GetWorld()->GetTimerManager().SetTimer(MultiSphereTraceTimer, this, &ACPPBaseCharacter::MultiSphereTrace, 0.2,
	                                       true);
}

void ACPPBaseCharacter::EndMultiTrace()
{
	GetWorld()->GetTimerManager().ClearTimer(MultiSphereTraceTimer);
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
				if (ItemActorRef != Hit.GetActor())
				{
					IGameplayTagAssetInterface* GameplayTagsInterface = Cast<IGameplayTagAssetInterface>(Hit.Actor);
					if (GameplayTagsInterface)
					{
						if (GameplayTagsInterface->HasMatchingGameplayTag(BoxCollection))
						{
							//Don't have to cast it everytime, create some tags that decribe what type of pickup it is
							//and then cast it when it is going to inventory cause casting here is not needed
							//also create gameplaytags system for which item exist in the inventory and which exist in the itemref/multiitemref
							IBoxItemInterface* castingItem = Cast<IBoxItemInterface>(Hit.Actor);
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
							IBaseItemInterface* castingItem = Cast<IBaseItemInterface>(Hit.Actor);
							if (castingItem)
							{
								ItemRef = castingItem;
								ItemActorRef = Hit.Actor;
								ItemRef->EnableWidgetVisibility(this);
							}
							else
							{
								ItemRef = nullptr;
								ItemActorRef = nullptr;
							}
						}
					}
				}
			}
			else
			{
				if (ItemRef != nullptr)
				{
					ItemRef->DisableWidgetVisibility(this);
					//ServerItemRemovePawn(ItemRef, PlayerControllerRef);
					ItemRef = nullptr;
				}
			}
		}
	}
	else
	{
		if (ItemRef != nullptr)
		{
			ItemRef->DisableWidgetVisibility(this);
			//ServerItemRemovePawn(ItemRef);
			ItemRef = nullptr;
		}
	}
}

void ACPPBaseCharacter::MultiSphereTrace()
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
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_GameTraceChannel2,
	                                             MyColSphere);

	if (isHit)
	{
		if (HitResultArrayToCompare.Num() > 0)
		{
			if (HitResultArrayToCompare.Num() == OutHits.Num())
			{
				MultiItemRef.Empty();
				//HitResultArrayToCompare.Empty();

				bool ArrayNotMatch = false;
				TArray<AActor*> LocalActorListFromTrace;
				// loop through TArray
				for (int32 i = 0; i < OutHits.Num(); i++)
				{
					if (!(HitResultArrayToCompare.Contains(OutHits[i].GetActor())))
					{
						IGameplayTagAssetInterface* GameplayTagsInterface = Cast<IGameplayTagAssetInterface>(
							OutHits[i].Actor);
						if (GameplayTagsInterface)
						{
							if (!(GameplayTagsInterface->HasMatchingGameplayTag(BoxCollection)))
							{
								//No Need, but the single trace has to be cast in order to display widget to the character
								IMasterItemInterface* castingItem = Cast<IMasterItemInterface>(OutHits[i].Actor);
								if (castingItem)
								{
									MultiItemRef.Add(castingItem);
									LocalActorListFromTrace.Add(OutHits[i].GetActor());
									ArrayNotMatch = true;
								}
							}
						}
						
					}

				}

				if (ArrayNotMatch)
				{
					PCInterfaceRef->RefreshInventory();
					HitResultArrayToCompare = LocalActorListFromTrace;
				}
			}
			else
			{
				MultiItemRef.Empty();
				HitResultArrayToCompare.Empty();

				// loop through TArray
				for (int32 i = 0; i < OutHits.Num(); i++)
				{
					IGameplayTagAssetInterface* GameplayTagsInterface = Cast<IGameplayTagAssetInterface>(
						OutHits[i].Actor);
					if (GameplayTagsInterface)
					{
						if (!(GameplayTagsInterface->HasMatchingGameplayTag(BoxCollection)))
						{
							//No Need, but the single trace has to be cast in order to display widget to the character
							IMasterItemInterface* castingItem = Cast<IMasterItemInterface>(OutHits[i].Actor);
							if (castingItem)
							{
								MultiItemRef.Add(castingItem);
								HitResultArrayToCompare.Add(OutHits[i].GetActor());
							}
						}
					}
				}
			}
		}
		else
		{
			MultiItemRef.Empty();
			HitResultArrayToCompare.Empty();

			// loop through TArray
			for (auto& Hit : OutHits)
			{
				IGameplayTagAssetInterface* GameplayTagsInterface = Cast<IGameplayTagAssetInterface>(Hit.Actor);
				if (GameplayTagsInterface)
				{
					if (!(GameplayTagsInterface->HasMatchingGameplayTag(BoxCollection)))
					{
						//No Need, but the single trace has to be cast in order to display widget to the character
						IMasterItemInterface* castingItem = Cast<IMasterItemInterface>(Hit.Actor);
						if (castingItem)
						{
							MultiItemRef.Add(castingItem);
							HitResultArrayToCompare.Add(Hit.GetActor());
						}
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

bool ACPPBaseCharacter::ServerItemAddPawn_Validate(AMasterItem* itemRefParam)
{
	if (IsValid(itemRefParam))
	{
		return true;
	}
	return false;
}

void ACPPBaseCharacter::ServerItemAddPawn_Implementation(AMasterItem* itemRefParam)
{
	//itemRefParam->ServerAddPawnRef(PCRefParam);
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
	//itemRefParam->ServerRemovePawnRef(PCRefParam);
}

void ACPPBaseCharacter::CheckForGroundTrace(FVector& End, bool& bIsHit, FVector& SpawnVectorParam)
{
	// create tarray for hit results
	FHitResult OutHits;

	float xMin = End.X - 50;
	float xMax = End.X + 50;

	float RandomX = FMath::RandRange(xMin, xMax);

	float yMin = End.Y - 50;
	float yMax = End.Y + 50;

	float RandomY = FMath::RandRange(yMin, yMax);

	// start and end locations
	FVector SweepStart = FVector(RandomX, RandomY, End.Z + 1000);
	FVector SweepEnd = FVector(RandomX, RandomY, End.Y);

	// create a collision sphere
	FCollisionShape MyColSphere = FCollisionShape::MakeCapsule(40.0f, 1000.0f);

	// draw collision sphere
	DrawDebugCapsule(GetWorld(), End, MyColSphere.GetCapsuleHalfHeight(), MyColSphere.GetCapsuleRadius(),
	                 this->GetActorRotation().Quaternion(), FColor::Purple, true);

	// check if something got hit in the sweep
	bool isHit = GetWorld()->SweepSingleByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_GameTraceChannel2,
	                                              MyColSphere);

	if (isHit)
	{
		bIsHit = isHit;
		SpawnVectorParam = OutHits.GetActor()->GetActorLocation();
	}
}

float ACPPBaseCharacter::CalculateCurrentWeight()
{
	float LocalCurrentWeight = 0;
	for (const FItemData& LoopItemData : Inventory)
	{
		LocalCurrentWeight = LocalCurrentWeight + (LoopItemData.Amount * LoopItemData.Weight);
	}
	return LocalCurrentWeight;
}

void ACPPBaseCharacter::CalledWhenPossessedIsCalled(AController* NewController)
{
	PlayerControllerRef = Cast<ACPPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ACPPBaseCharacter::TakeMasterItem(AMasterItem* MasterItemRefParam)
{
	MultiItemRef.Remove(MasterItemRefParam);
}

bool ACPPBaseCharacter::AddItemToInventory(const FItemData& ItemDataParam)
{
	if ((CurrentWeight + (ItemDataParam.Amount * ItemDataParam.Weight)) <= MaxWeight)
	{
		if (ItemDataParam.bIsStackable)
		{
			int32 IndexNum;
			if (StackableItemExist(ItemDataParam.Name, IndexNum))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Black, "Stack");
				IncreaseAmountOnStackedItem(ItemDataParam, IndexNum);
			}
			else
			{
				Inventory.Add(ItemDataParam);
				CurrentWeight = CalculateCurrentWeight();
			}
			return true;
		}

		Inventory.Add(ItemDataParam);
		CurrentWeight = CalculateCurrentWeight();
		return true;
	}

	return false;
}

bool ACPPBaseCharacter::StackableItemExist(const FText& ItemNameParam, int32& IndexNumParam)
{
	for (int i = 0; i < Inventory.Num(); i++)
	{
		if (ItemNameParam.EqualTo(Inventory[i].Name))
		{
			IndexNumParam = i;
			return true;
		}
	}
	return false;
}

void ACPPBaseCharacter::IncreaseAmountOnStackedItem(const FItemData& ItemDataParam, int32& IndexNumParam)
{
	Inventory[IndexNumParam].IncreaseAmount(ItemDataParam.Amount);
}

bool ACPPBaseCharacter::CheckIfBoxCollectionIsNotNull() const
{
	if (BoxItemRef != nullptr)
	{
		return true;
	}

	return false;
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

void ACPPBaseCharacter::RemoveItemFromInventory(const int32& IndexNumParam)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, FString::FromInt(IndexNumParam));
	Inventory.RemoveAt(IndexNumParam);
}

void ACPPBaseCharacter::DropMasterItem(const FItemData& ItemDataParam)
{
	FVector SpawnLocation = this->GetActorLocation();
	//Make it a Random Forward Vector
	FVector SpawnForwardLocation = this->GetActorForwardVector() * -100;

	FVector SpawnActorTransform = SpawnLocation + SpawnForwardLocation;

	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	float xMin = SpawnActorTransform.X - 50;
	float xMax = SpawnActorTransform.X + 50;

	float RandomX = FMath::RandRange(xMin, xMax);

	float yMin = SpawnActorTransform.Y - 50;
	float yMax = SpawnActorTransform.Y + 50;

	float RandomY = FMath::RandRange(yMin, yMax);

	bool IsHit = false;
	FVector SpawnVector;

	CheckForGroundTrace(SpawnActorTransform, IsHit, SpawnVector);

	if (IsHit)
	{
		AMasterItem* DroppedItem = GetWorld()->SpawnActor<AMasterItem>(ItemDataParam.ClassOfItem, SpawnVector,
		                                                               FRotator(0, 0, 0), SpawnParameters);
		if (DroppedItem)
		{
			DroppedItem->SetItemData(ItemDataParam);
		}
	}
	else
	{
		AMasterItem* DroppedItem = GetWorld()->SpawnActor<AMasterItem>(ItemDataParam.ClassOfItem,
		                                                               FVector(RandomX,
		                                                                       RandomY, 200),
		                                                               FRotator(0, 0, 0), SpawnParameters);
		if (DroppedItem)
		{
			DroppedItem->SetItemData(ItemDataParam);
		}
	}
}

void ACPPBaseCharacter::DropBoxCollectionItem(const FItemData& ItemDataParam)
{
}

bool ACPPBaseCharacter::ItemExistInMultiRef(IMasterItemInterface* MasterItemInterfaceParam) const
{
	if (MultiItemRef.Contains(MasterItemInterfaceParam))
	{
		return true;
	}
	return false;
}

void ACPPBaseCharacter::RemoveFromMultiItemRef(IMasterItemInterface* MasterItemInterfaceParam)
{
	MultiItemRef.Remove(MasterItemInterfaceParam);

	PCInterfaceRef->RefreshInventory();
}

void ACPPBaseCharacter::ReScanMultiItemRef()
{
	MultiSphereTrace();
}

void ACPPBaseCharacter::DeleteNonValidReferenceFromMultiItemRef()
{
	//Probably just send that object reference to this function to delete the reference
}

void ACPPBaseCharacter::AddToInventory()
{
	if (ItemActorRef != nullptr)
	{
		AMasterItem* ItemToBeAdded = Cast<AMasterItem>(ItemActorRef);
		const FItemData ItemRefData = ItemToBeAdded->GetItemData();
		//GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Black, "Is this even called");
		if ((CurrentWeight + (ItemRefData.Amount * ItemRefData.Weight)) <= MaxWeight)
		{
			if (ItemRefData.bIsStackable)
			{
				int32 IndexNum;
				if (StackableItemExist(ItemRefData.Name, IndexNum))
				{
					//GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Black, "Stack");
					IncreaseAmountOnStackedItem(ItemRefData, IndexNum);
					DestroyMasterItem(ItemToBeAdded);
				}
				else
				{
					Inventory.Add(ItemRefData);
					CurrentWeight = CalculateCurrentWeight();
					DestroyMasterItem(ItemToBeAdded);
				}
			}
			else
			{
				Inventory.Add(ItemRefData);
				CurrentWeight = CalculateCurrentWeight();
				DestroyMasterItem(ItemToBeAdded);
			}
		}
	}
}
