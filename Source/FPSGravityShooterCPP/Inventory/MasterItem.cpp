// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterItem.h"
#include "FPSGravityShooterCPP/FrameWork/CPPPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

void AMasterItem::TakeItem_Implementation()
{
}

void AMasterItem::ServerDestroyItem_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("Server is called successfully"));
	for (AController* Ref : PCRefList)
	{
		Cast<ACPPPlayerController>(Ref)->RefreshInventory();
	}
	MulticastDestroyItem();
}

void AMasterItem::MulticastDestroyItem_Implementation()
{
	bool isBeingDestroyed = Destroy();
	if (isBeingDestroyed)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, TEXT("Is being destroyed"));
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
