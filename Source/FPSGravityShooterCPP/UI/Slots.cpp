// Fill out your copyright notice in the Description page of Project Settings.


#include "Slots.h"
#include "HoverWidget.h"

void USlots::SetToolTipWidget()
{
	UHoverWidget* HoverWidget = CreateWidget<UHoverWidget>(this, HoverWidgetClassRef);
	HoverWidget->SetName(ItemData.Name);
	HoverWidget->SetDescription(ItemData.Description);
	HoverWidget->SetWeight(ItemData.Amount * ItemData.Weight);
	HoverWidget->SetImage(ItemData.Image);
	SetToolTip(HoverWidget);
}

void USlots::SetItemAmountText()
{
	if (ItemData.bIsStackable)
	{
		if (ItemData.Amount > 1)
		{
			FString AmountText = FString::FromInt(ItemData.Amount);
			AmountText.Append(" X ");
			ItemAmountText->SetText(FText::FromString(AmountText));
		}
		else
		{
			ItemAmountText->SetText(FText::FromString(""));
		}
	}
	else
	{
		ItemAmountText->SetText(FText::FromString(""));
	}
}

void USlots::SetNameText()
{
	NameText->SetText(ItemData.Name);
}
