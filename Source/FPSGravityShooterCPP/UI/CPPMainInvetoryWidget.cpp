// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPMainInvetoryWidget.h"
#include "Components/ScrollBox.h"
#include "FPSGravityShooterCPP/FrameWork/CPPBaseCharacter.h"
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

	int32 InventoryIndexNum = 0;
	for (const FItemData& item : PawnRef->GetInventory())
	{
		UItemSlot* slot = CreateWidget<UItemSlot>(GetWorld(), ItemSlotClassRef);
		slot->SetItemData(item);
		slot->SetNameText();
		slot->SetItemAmountText();
		slot->SetPawnRef(PawnRef);
		slot->SetPCRef(PCRef);
		slot->SetIndexNum(InventoryIndexNum);
		slot->SetToolTipWidget();
		InventoryItemContainer->AddChild(slot);

		InventoryIndexNum = InventoryIndexNum + 1;
	}

	float CurrentWeightNum = PawnRef->GetCurrentWeight();
	FString CurrentWeightString = FString::SanitizeFloat(CurrentWeightNum);
	FString MaxWeightString = FString::SanitizeFloat(PawnRef->GetMaxWeight());
	FString WeightAppendText = CurrentWeightString + " / " + MaxWeightString;
	WeightText->SetText(FText::FromString(WeightAppendText));
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
		slot->SetItemAmountText();
		slot->SetPawnRef(PawnRef);
		slot->SetMasterItemRef(Ref);
		slot->SetPCRef(PCRef);
		slot->SetToolTipWidget();
		GroundItemContainer->AddChild(slot);
	}
}
