// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPBaseItem.h"
#include "FPSGravityShooterCPP/Interfaces/BoxItemInterface.h"
#include "BoxCollectionItem.generated.h"

/**
 *
 */
UCLASS()
class FPSGRAVITYSHOOTERCPP_API ABoxCollectionItem : public ACPPBaseItem, public IBoxItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void TakeItem(int32 ItemDataIndexNum) override;

	UFUNCTION(Server, Reliable, Category = "PickupItem")
	void ServerTakeItem(int32 ItemDataIndexNum);
	void ServerTakeItem_Implementation(int32 ItemDataIndexNum);

	UFUNCTION(Server, Reliable, Category = "PickupItem")
	void DestroyItem();
	void DestroyItem_Implementation();

	virtual const TArray<FItemData>& GetItemDataList() const override
	{
		return ItemDataList;
	}

	void SetItemDataList(const TArray<FItemData>& ItemDataListRef)
	{
		this->ItemDataList = ItemDataListRef;
	}

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void CheckListLength();

	UPROPERTY(Replicated)
	TArray<FItemData> ItemDataList;

	void RefreshList();

	UFUNCTION(Server, Reliable)
	void ServerRefreshList();
	void ServerRefreshList_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCastRefreshList();
	void MultiCastRefreshList_Implementation();
};
