// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSGravityShooterCPP/Inventory/CPPBaseItem.h"
#include "InteractableWidget.generated.h"

class UBorder;
class UTextBlock;
/**
 * 
 */
UCLASS()
class FPSGRAVITYSHOOTERCPP_API UInteractableWidget : public UUserWidget
{
	GENERATED_BODY()

	FItemData ItemData;

public:
	void SetItemData(const FItemData& ItemDataParam)
	{
		ItemData = ItemDataParam;
	}

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<EItemRarity, UTexture2D*> RarityColors;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* TopFadeBG;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* CenterFadeBG;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* CenterBottomFadeBG;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* BottomVerticalLine;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* BottomLine;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemAmount;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemCategory;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemRarity;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class USizeBox* ItemIconContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* ItemIcon;
};
