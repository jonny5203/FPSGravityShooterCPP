// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterItem.h"

#include "GameplayTagsManager.h"
#include "FPSGravityShooterCPP/FrameWork/CPPPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

//
//	Expand the gameplay tags alittle so that I will know exactly what item this is instead of just the category
//

void AMasterItem::TakeItem_Implementation()
{
}

void AMasterItem::ServerDestroyItem_Implementation()
{
	MulticastDestroyItem();
}

void AMasterItem::MulticastDestroyItem_Implementation()
{
	//NOTE: (This May Be Important)Maybe also just measure distance of actors to know where they are to calcualate if there is a chance they have it inside their inventory
	//then I don't have to check everytime if it's inside the inventory or not
	//When sending information to the item for display of widget, just send in an interface instead
	//bool bShouldDeleteRefFromCharacter;
	
	IGameplayTagAssetInterface* ControllerTagInterface = Cast<IGameplayTagAssetInterface>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (ControllerTagInterface)
	{
		if (ControllerTagInterface->HasMatchingGameplayTag(
				UGameplayTagsManager::Get().RequestGameplayTag("InventoryState.ReferenceExist.true"))
			&& ControllerTagInterface->HasMatchingGameplayTag(
				UGameplayTagsManager::Get().RequestGameplayTag("InventoryState.IsInInventory.false")))
		{
			//Just tell the character to update it's item or multiitemref
			//Correct it so that the player will call the player controller for refreshing of inventory hud

			ACharacter* LocalCharacterRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			if (GetDistanceTo(LocalCharacterRef) <= 500.f)
			{
				//May just be better to just do this in the character class instead
				ICharacterInterface* LocalCharacterInterfaceRef = Cast<ICharacterInterface>(LocalCharacterRef);
				LocalCharacterInterfaceRef->ReScanMultiItemRef();
			}
		}
	}
	
	const bool isBeingDestroyed = Destroy();
	if (isBeingDestroyed)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("Is being destroyed"));
		if (true)
		{
			
		}
	}
}

void AMasterItem::DestroyItem()
{
	ServerDestroyItem();
}

void AMasterItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMasterItem, ItemData);
}
