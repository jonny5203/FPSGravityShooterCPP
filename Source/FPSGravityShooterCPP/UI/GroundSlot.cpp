// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundSlot.h"
#include "Components/Button.h"
#include "FPSGravityShooterCPP/FrameWork/CPPBaseCharacter.h"
#include "FPSGravityShooterCPP/FrameWork/CPPPlayerController.h"
#include "FPSGravityShooterCPP/Inventory/MasterItem.h"

void UGroundSlot::NativeConstruct()
{
	TakeButton->OnClicked.AddDynamic(this, &UGroundSlot::AddItemToInventory);
}

void UGroundSlot::AddItemToInventory()
{
	/*
	if(PawnRef->AddItemToInventory(ItemData))
	{
		//PawnRef->GetMultiItemRef().Remove(MasterItemRef); Needed for when the another player has the inventory open when you take the item with F key
		//Maybe just call destroy directly in here rather than calling pawn and then back
		PawnRef->DestroyMasterItem(MasterItemRef); 
	}
	*/
}