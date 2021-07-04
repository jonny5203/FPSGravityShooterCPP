// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSGravityShooterCPP/Interfaces/CharacterInterface.h"

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

	ICharacterInterface* GetPawnRef() const
	{
		return PawnRef;
	}

	void SetPawnRef(ICharacterInterface* PawnRefParam)
	{
		this->PawnRef = PawnRefParam;
	}

	void SetPawnRefToNullptr()
	{
		this->PawnRef = nullptr;
	}

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* InventoryItemContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* GroundItemContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* WeightText;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UItemSlot> ItemSlotClassRef;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGroundSlot> GroundSlotClassRef;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UGroundBoxSlot> GroundBoxSlotClassRef;

private:
	ICharacterInterface* PawnRef;
};
