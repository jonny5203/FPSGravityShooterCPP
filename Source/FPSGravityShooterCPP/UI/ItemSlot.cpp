// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlot.h"
#include "Components/Button.h"
#include "FPSGravityShooterCPP/FrameWork/CPPBaseCharacter.h"
#include "FPSGravityShooterCPP/FrameWork/CPPPlayerController.h"

void UItemSlot::NativeConstruct()
{
	DropButton->OnClicked.AddDynamic(this, &UItemSlot::DropItem);
}

void UItemSlot::DropItem()
{
	//Spawn a new item on the map and delete the references of the item in the characters inventory
	//PawnRef->RemoveItemFromInventory(IndexNum);
	PCRef->RefreshInventoryMasterItemDrop(IndexNum, ItemData);
	//PawnRef->DropMasterItem(ItemData);
}
