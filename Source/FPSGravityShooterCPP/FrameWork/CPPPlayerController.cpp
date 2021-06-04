// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPPlayerController.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "FPSGravityShooterCPP//UI/CPPMainInvetoryWidget.h"
#include "FPSGravityShooterCPP/Interfaces/CharacterInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


ACPPPlayerController::ACPPPlayerController()
{

	static ConstructorHelpers::FClassFinder<UCPPMainInvetoryWidget> mainInventoryObj(TEXT("/Game/FPS/UI/Inventory/WB_MainInventory"));
	if (mainInventoryObj.Succeeded())
	{
		MainInventoryClassRef = mainInventoryObj.Class;
	}

}

void ACPPPlayerController::OpenInventory()
{
	if (bIsInInventory)
	{
		if (IsValid(MainInventoryWidgetRef))
		{
			Cast<ICharacterInterface>(GetCharacter())->StartMultiTrace();
			
			MainInventoryWidgetRef->SetVisibility(ESlateVisibility::Visible);

			SetShowMouseCursor(true);
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, MainInventoryWidgetRef);
			bIsInInventory = !bIsInInventory;

			

			MainInventoryWidgetRef->BuildInventory();
		}
		else
		{
			Cast<ICharacterInterface>(GetCharacter())->StartMultiTrace();
			
			MainInventoryWidgetRef = CreateWidget<UCPPMainInvetoryWidget>(this, MainInventoryClassRef);
			MainInventoryWidgetRef->AddToViewport();

			SetShowMouseCursor(true);
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, MainInventoryWidgetRef);
			bIsInInventory = !bIsInInventory;
		}
	}
	else
	{
		MainInventoryWidgetRef->SetVisibility(ESlateVisibility::Hidden);
		SetShowMouseCursor(false);
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		bIsInInventory = !bIsInInventory;
	}
}

void ACPPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("ShowScoreScreen"), IE_Pressed, this, &ACPPPlayerController::OpenInventory);
}
