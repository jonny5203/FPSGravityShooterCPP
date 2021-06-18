// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HoverWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSGRAVITYSHOOTERCPP_API UHoverWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Name;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Description;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Weight;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* DisplayImage;

public:
	void SetName(const FText& NameParam);
	
	void SetDescription(const FText& DescriptionParam);

	void SetWeight(const float& WeightParam);

	void SetImage(class UTexture2D* ImageParam);
};
