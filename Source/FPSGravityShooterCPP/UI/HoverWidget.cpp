// Fill out your copyright notice in the Description page of Project Settings.


#include "HoverWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"

void UHoverWidget::SetName(const FText& NameParam)
{
	Name->SetText(NameParam);
}

void UHoverWidget::SetDescription(const FText& DescriptionParam)
{
	Description->SetText(DescriptionParam);
}

void UHoverWidget::SetWeight(const float& WeightParam)
{
	FString FloatToString = FString::SanitizeFloat(WeightParam);
	FloatToString.Append(" KG");
	Weight->SetText(FText::FromString(FloatToString));
}

void UHoverWidget::SetImage(UTexture2D* ImageParam)
{
	DisplayImage->SetBrushFromTexture(ImageParam);
}
