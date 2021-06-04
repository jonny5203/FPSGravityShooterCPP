// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPBaseItem.h"
#include "BoxCollectionItem.generated.h"

/**
 *
 */
UCLASS()
class FPSGRAVITYSHOOTERCPP_API ABoxCollectionItem : public ACPPBaseItem
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable, Category = "PickupItem")
	void TakeItem(const FItemData& item);
	void TakeItem_Implementation(const FItemData& item);

	UFUNCTION(Server, Reliable, Category = "PickupItem")
	void DestroyItem();
	void DestroyItem_Implementation();

	const TArray<FItemData>& GetItemDataList() const
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
};
