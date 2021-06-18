// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slots.h"
#include "ItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class FPSGRAVITYSHOOTERCPP_API UItemSlot : public USlots
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* DropButton;

private:
	UFUNCTION()
	void DropItem();

	UPROPERTY()
	int32 IndexNum;
	
public:
	int32 GetIndexNum() const
	{
		return IndexNum;
	}

	void SetIndexNum(const int32& IndexNumParam)
	{
		this->IndexNum = IndexNumParam;
	}
};
