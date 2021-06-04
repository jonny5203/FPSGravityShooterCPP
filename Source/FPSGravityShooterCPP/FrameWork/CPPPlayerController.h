// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPPPlayerController.generated.h"

class UCPPMainInvetoryWidget;

/**
 *
 */
UCLASS(BlueprintType)
class FPSGRAVITYSHOOTERCPP_API ACPPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACPPPlayerController();

private:
	UFUNCTION()
		void OpenInventory();

	virtual void SetupInputComponent() override;

	UPROPERTY()
		bool bIsInInventory = true;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		TSubclassOf<UCPPMainInvetoryWidget> MainInventoryClassRef;

	UPROPERTY()
		UCPPMainInvetoryWidget* MainInventoryWidgetRef;
};
