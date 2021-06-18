// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "FPSGravityShooterCPP/Inventory/CPPBaseItem.h"
#include "Components/TextBlock.h"
#include "Slots.generated.h"

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

	UPROPERTY()
	ACPPBaseCharacter* PawnRef;

	UPROPERTY()
	class AMasterItem* MasterItemRef;

	UPROPERTY()
	ACPPPlayerController* PCRef;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHoverWidget> HoverWidgetClassRef;

public:
	void SetToolTipWidget();
	
	void SetItemAmountText();

	ACPPPlayerController* GetPCRef() const
	{
		return PCRef;
	}

	void SetPCRef(ACPPPlayerController* PCRefParam)
	{
		this->PCRef = PCRefParam;
	}

	ACPPBaseCharacter* GetPawnRef() const
	{
		return PawnRef;
	}

	void SetPawnRef(ACPPBaseCharacter* PawnRefParam)
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

	AMasterItem* GetMasterItemRef() const
	{
		return MasterItemRef;
	}

	void SetMasterItemRef(AMasterItem* MasterItemRefParam)
	{
		this->MasterItemRef = MasterItemRefParam;
	}

	void SetNameText();
};
