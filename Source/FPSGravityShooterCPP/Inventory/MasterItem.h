// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPBaseItem.h"
#include "FPSGravityShooterCPP/Interfaces/MasterItemInterface.h"
#include "MasterItem.generated.h"

/**
 *
 */
UCLASS()
class FPSGRAVITYSHOOTERCPP_API AMasterItem : public ACPPBaseItem, public IMasterItemInterface
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

	virtual void DestroyItem() override;

	virtual FItemData GetItemData() const override
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

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WhichItem;
};
