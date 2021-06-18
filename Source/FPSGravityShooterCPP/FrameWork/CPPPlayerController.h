// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPPBaseCharacter.h"
#include "CPPPlayerController.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class FPSGRAVITYSHOOTERCPP_API ACPPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACPPPlayerController();

	UFUNCTION()
	void RefreshInventoryMasterItemTake(AMasterItem* MasterItemRefParam);

	UFUNCTION()
	void RefreshInventoryMasterItemDrop(int32 IndexNumParam, const FItemData& ItemDataParam);

	UFUNCTION()
	void ResetPawnRef();

protected:
	UFUNCTION()
	void OpenInventory();

	virtual void SetupInputComponent() override;

	UPROPERTY()
	bool bIsInInventory = true;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UCPPMainInvetoryWidget> MainInventoryClassRef;

	UPROPERTY()
	UCPPMainInvetoryWidget* MainInventoryWidgetRef;

private:
	UPROPERTY()
	ACPPBaseCharacter* PawnRef;
};
