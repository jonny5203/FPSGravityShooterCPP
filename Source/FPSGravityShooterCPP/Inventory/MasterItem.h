// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPBaseItem.h"
#include "MasterItem.generated.h"

/**
 *
 */
UCLASS()
class FPSGRAVITYSHOOTERCPP_API AMasterItem : public ACPPBaseItem
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable, Category = "PickupItem")
	void TakeItem();
	void TakeItem_Implementation();

	UFUNCTION(Server, Reliable, Category = "PickupItem")
	void ServerDestroyItem();
	void ServerDestroyItem_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDestroyItem();
	void MulticastDestroyItem_Implementation();

	void DestroyItem();

	FItemData GetItemData() const
	{
		return ItemData;
	}

	void SetItemData(const FItemData& ItemDataRef)
	{
		this->ItemData = ItemDataRef;
	}

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, EditDefaultsOnly)
	FItemData ItemData;
};
