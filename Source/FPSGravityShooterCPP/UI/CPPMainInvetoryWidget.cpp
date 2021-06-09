// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPMainInvetoryWidget.h"
#include "Components/ScrollBox.h"
#include "FPSGravityShooterCPP/FrameWork/CPPBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ItemSlot.h"
#include "GroundSlot.h"
#include "GroundBoxSlot.h"
#include "Slots.h"
#include "FPSGravityShooterCPP/FrameWork/CPPPlayerController.h"

void UCPPMainInvetoryWidget::NativeConstruct()
{
	PawnRef = Cast<ACPPBaseCharacter>(GetOwningPlayerPawn());
	PCRef = Cast<ACPPPlayerController>(GetOwningPlayer());

	BuildInventory();
	BuildGroundItems();
}

void UCPPMainInvetoryWidget::RebuildInventory()
{
	BuildInventory();
	BuildGroundItems();
}

void UCPPMainInvetoryWidget::BuildInventory()
{
	InventoryItemContainer->ClearChildren();

	for (const FItemData& item : PawnRef->GetInventory())
	{
		USlots* slot = CreateWidget<USlots>(GetWorld(), ItemSlotClassRef);
		slot->SetItemData(item);
		slot->SetNameText();
		slot->SetPawnRef(PawnRef);
		slot->SetPCRef(PCRef);
		InventoryItemContainer->AddChild(slot);
	}
}

void UCPPMainInvetoryWidget::BuildGroundItems()
{
	GroundItemContainer->ClearChildren();

	for (AMasterItem* Ref : PawnRef->GetMultiItemRef())
	{
		const FItemData& item = Ref->GetItemData();

		USlots* slot = CreateWidget<USlots>(GetWorld(), GroundSlotClassRef);
		slot->SetItemData(item);
		slot->SetNameText();
		slot->SetPawnRef(PawnRef);
		slot->SetMasterItemRef(Ref);
		slot->SetPCRef(PCRef);
		GroundItemContainer->AddChild(slot);
	}
}
