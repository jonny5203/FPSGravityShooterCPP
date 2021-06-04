// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxCollectionItem.h"
#include "Net/UnrealNetwork.h"

void ABoxCollectionItem::TakeItem_Implementation(const FItemData& item)
{
	ItemDataList.Remove(item);

	if (ItemDataList.Num() <= 0)
	{
		RefreshList();
		Destroy();
	}
	else
	{
		RefreshList();
	}
}

void ABoxCollectionItem::DestroyItem_Implementation()
{
	RefreshList();
	Destroy();
}

void ABoxCollectionItem::CheckListLength()
{
	if (ItemDataList.Num() == 0)
	{
		DestroyItem();
	}
}

void ABoxCollectionItem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABoxCollectionItem, ItemDataList);
}
