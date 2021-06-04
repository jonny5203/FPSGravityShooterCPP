// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPMainInvetoryWidget.h"
#include "Components/ScrollBox.h"
#include "FPSGravityShooterCPP/FrameWork/CPPBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ItemSlot.h"
#include "GroundSlot.h"
#include "GroundBoxSlot.h"
#include "Slots.h"

void UCPPMainInvetoryWidget::NativeConstruct()
{
	PawnRef = Cast<ACPPBaseCharacter>(GetOwningPlayerPawn());

	BuildInventory();
}

void UCPPMainInvetoryWidget::BuildInventory()
{
	InventoryItemContainer->ClearChildren();

	for (FItemData item : PawnRef->GetInventory())
	{
		USlots* slot = CreateWidget<USlots>(GetWorld(), ItemSlotClassRef);
		slot->SetItemData(item);
		slot->SetNameText();
		InventoryItemContainer->AddChild(slot);
	}
}
