// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPPMainInvetoryWidget.generated.h"

/**
 *
 */
UCLASS()
class FPSGRAVITYSHOOTERCPP_API UCPPMainInvetoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void BuildInventory();
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UScrollBox* InventoryItemContainer;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UItemSlot> ItemSlotClassRef;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UGroundSlot> GroundSlotClassRef;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UGroundBoxSlot> GroundBoxSlotClassRef;

private:
	UPROPERTY()
		class ACPPBaseCharacter* PawnRef;
};
