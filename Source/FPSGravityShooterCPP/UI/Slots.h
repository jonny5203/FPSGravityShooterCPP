// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "FPSGravityShooterCPP/Inventory/CPPBaseItem.h"
#include "Components/TextBlock.h"
#include "FPSGravityShooterCPP/Interfaces/CharacterInterface.h"
#include "Slots.generated.h"

class IMasterItemInterface;
class ACPPPlayerController;
class UTextBlock;
class UHoverWidget;
/**
 * 
 */
UCLASS()
class FPSGRAVITYSHOOTERCPP_API USlots : public UUserWidget
{
	GENERATED_BODY()

protected:
	FItemData ItemData;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemAmountText;

	ICharacterInterface* PawnRef;

	IMasterItemInterface* MasterItemRef;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHoverWidget> HoverWidgetClassRef;

public:
	void SetToolTipWidget();
	
	void SetItemAmountText();

	const ICharacterInterface* GetPawnRef() const
	{
		return PawnRef;
	}

	void SetPawnRef(ICharacterInterface* PawnRefParam)
	{
		this->PawnRef = PawnRefParam;
	}

	FItemData GetItemData() const
	{
		return ItemData;
	}

	void SetItemData(const FItemData& itemDataObject)
	{
		ItemData = itemDataObject;
	}

	const IMasterItemInterface* GetMasterItemRef() const
	{
		return MasterItemRef;
	}

	void SetMasterItemRef(IMasterItemInterface* MasterItemRefParam)
	{
		this->MasterItemRef = MasterItemRefParam;
	}

	void SetNameText();
};
