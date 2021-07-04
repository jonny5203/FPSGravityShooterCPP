// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSGravityShooterCPP/Inventory/CPPBaseItem.h"
#include "UObject/Interface.h"
#include "MasterItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMasterItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSGRAVITYSHOOTERCPP_API IMasterItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void DestroyItem() = 0;

	virtual FItemData GetItemData() const = 0;
};
