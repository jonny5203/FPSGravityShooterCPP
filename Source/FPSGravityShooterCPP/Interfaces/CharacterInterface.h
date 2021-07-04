// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSGravityShooterCPP/Inventory/CPPBaseItem.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

class IMasterItemInterface;
class AMasterItem;
class IBoxItemInterface;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSGRAVITYSHOOTERCPP_API ICharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void StartMultiTrace() = 0;

	UFUNCTION()
	virtual void EndMultiTrace() = 0;

	virtual void DropMasterItem(const FItemData& ItemDataParam) = 0;

	virtual void DropBoxCollectionItem(const FItemData& ItemDataParam) = 0;

	virtual void TakeMasterItem(AMasterItem* MasterItemRefParam) = 0;

	virtual const TArray<FItemData>& GetInventory() const = 0;

	virtual const TArray<IMasterItemInterface*>& GetMultiItemRef() const = 0;

	virtual const float& GetMaxWeight() const = 0;

	virtual const float& GetCurrentWeight() const = 0;

	virtual void RemoveItemFromInventory(const int32& IndexNumParam) = 0;

	virtual bool CheckIfBoxCollectionIsNotNull() const = 0;

	virtual IBoxItemInterface* GetBoxItemRef() const = 0;

	virtual bool ItemExistInMultiRef(IMasterItemInterface* MasterItemInterfaceParam) const = 0;

	virtual void RemoveFromMultiItemRef(IMasterItemInterface* MasterItemInterfaceParam) = 0;

	virtual void ReScanMultiItemRef() = 0;

	virtual void DeleteNonValidReferenceFromMultiItemRef() = 0;

	virtual bool GetbDead() const = 0;

	virtual void SetbDead(bool bDeadParam) = 0;
};