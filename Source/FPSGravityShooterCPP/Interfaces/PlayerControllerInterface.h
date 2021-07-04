// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSGravityShooterCPP/Inventory/CPPBaseItem.h"
#include "UObject/Interface.h"
#include "PlayerControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSGRAVITYSHOOTERCPP_API IPlayerControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void RefreshInventory() = 0;

	UFUNCTION()
	virtual void RefreshInventoryMasterItemTake(AMasterItem* MasterItemRefParam) = 0;

	UFUNCTION()
	virtual void RefreshInventoryMasterItemDrop(int32 IndexNumParam, const FItemData& ItemDataParam) = 0;

	UFUNCTION()
	virtual bool GetIsInInventory() const = 0;

	UFUNCTION()
	virtual void ResetPawnRef() = 0;

	UFUNCTION()
	virtual void SetPawnInterfaceRef() = 0;
};
