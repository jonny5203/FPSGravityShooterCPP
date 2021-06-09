// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPPMainInvetoryWidget.generated.h"

class ACPPPlayerController;
class ACPPBaseCharacter;
class UScrollBox;

/**
 *
 */
UCLASS()
class FPSGRAVITYSHOOTERCPP_API UCPPMainInvetoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void RebuildInventory();

	UFUNCTION()
	void BuildInventory();

	UFUNCTION()
	void BuildGroundItems();

	ACPPBaseCharacter* GetPawnRef() const
	{
		return PawnRef;
	}

	void SetPawnRef(ACPPBaseCharacter* PawnRefParam)
	{
		this->PawnRef = PawnRefParam;
	}

	void SetPawnRefToNullptr()
	{
		this->PawnRef = nullptr;
	}

	ACPPPlayerController* GetPCRef() const
	{
		return PCRef;
	}

	void SetPCRef(ACPPPlayerController* PCRefParam)
	{
		this->PCRef = PCRefParam;
	}

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* InventoryItemContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* GroundItemContainer;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UItemSlot> ItemSlotClassRef;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGroundSlot> GroundSlotClassRef;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGroundBoxSlot> GroundBoxSlotClassRef;

private:
	UPROPERTY()
	ACPPBaseCharacter* PawnRef;

	UPROPERTY()
	ACPPPlayerController* PCRef;
};
