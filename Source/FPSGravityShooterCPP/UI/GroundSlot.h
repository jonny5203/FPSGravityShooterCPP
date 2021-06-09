// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slots.h"
#include "GroundSlot.generated.h"

/**
 * 
 */
UCLASS()
class FPSGRAVITYSHOOTERCPP_API UGroundSlot : public USlots
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* TakeButton;

private:
	UFUNCTION()
	void AddItemToInventory();
};
