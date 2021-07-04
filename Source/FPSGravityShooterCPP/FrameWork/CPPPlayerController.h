// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPPBaseCharacter.h"
#include "FPSGravityShooterCPP/Interfaces/PlayerControllerInterface.h"
#include "CPPPlayerController.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class FPSGRAVITYSHOOTERCPP_API ACPPPlayerController : public APlayerController, public IGameplayTagAssetInterface,
                                                      public IPlayerControllerInterface
{
	GENERATED_BODY()

public:
	ACPPPlayerController();

	UFUNCTION()
	virtual void RefreshInventoryMasterItemTake(AMasterItem* MasterItemRefParam) override;

	UFUNCTION()
	virtual void RefreshInventoryMasterItemDrop(int32 IndexNumParam, const FItemData& ItemDataParam) override;

	UFUNCTION()
	virtual void ResetPawnRef() override;

	UFUNCTION()
	virtual void SetPawnInterfaceRef() override;

	UFUNCTION()
	virtual void RefreshInventory() override;

	UFUNCTION(BlueprintPure)
	virtual bool GetIsInInventory() const override
	{
		return bIsInInventory;
	}

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override
	{
		TagContainer = GameplayTags;
		return;
	}

	void SetIsInInventory(bool bIsInInventoryParam)
	{
		this->bIsInInventory = bIsInInventoryParam;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
	FGameplayTagContainer GameplayTags;

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

	//UPROPERTY()
	ICharacterInterface* PawnInterfaceRef;
};
