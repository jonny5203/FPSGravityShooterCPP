// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSGravityShooterCPP/Inventory/CPPBaseItem.h"
#include "UObject/Interface.h"
#include "BoxItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBoxItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSGRAVITYSHOOTERCPP_API IBoxItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual const TArray<FItemData>& GetItemDataList() const = 0;

	virtual void TakeItem(int32 ItemDataIndexNum) = 0;
};
