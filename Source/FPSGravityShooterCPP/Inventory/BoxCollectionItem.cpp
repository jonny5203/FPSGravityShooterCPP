// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxCollectionItem.h"
#include "Net/UnrealNetwork.h"

//Create gameplay tags for all the items in this collection for easier determine what it is

void ABoxCollectionItem::TakeItem(int32 ItemDataIndexNum)
{
	ServerTakeItem(ItemDataIndexNum);
}

void ABoxCollectionItem::ServerTakeItem_Implementation(int32 ItemDataIndexNum)
{
	ItemDataList.RemoveAt(ItemDataIndexNum);

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

void ABoxCollectionItem::RefreshList()
{

}

void ABoxCollectionItem::ServerRefreshList_Implementation()
{

}

void ABoxCollectionItem::MultiCastRefreshList_Implementation()
{

}

void ABoxCollectionItem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABoxCollectionItem, ItemDataList);
}
