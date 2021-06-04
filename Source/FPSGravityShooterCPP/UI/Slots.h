// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "FPSGravityShooterCPP/Inventory/CPPBaseItem.h"
#include "Components/TextBlock.h"
#include "Slots.generated.h"

/**
 * 
 */
UCLASS()
class FPSGRAVITYSHOOTERCPP_API USlots : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	FItemData ItemData;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* NameText;


public:
	FItemData GetItemData() const
	{
		return ItemData;
	}

	void SetItemData(const FItemData& itemDataObject)
	{
		ItemData = itemDataObject;
	}

	void SetNameText();
};
