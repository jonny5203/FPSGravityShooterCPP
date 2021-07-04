// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "FPSGravityShooterCPP//UI/CPPMainInvetoryWidget.h"
#include "FPSGravityShooterCPP/Interfaces/CharacterInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "CPPBaseCharacter.h"


ACPPPlayerController::ACPPPlayerController()
{
	static ConstructorHelpers::FClassFinder<UCPPMainInvetoryWidget> mainInventoryObj(
		TEXT("/Game/FPS/UI/Inventory/WB_MainInventory"));
	if (mainInventoryObj.Succeeded())
	{
		MainInventoryClassRef = mainInventoryObj.Class;
	}
}

void ACPPPlayerController::RefreshInventoryMasterItemTake(AMasterItem* MasterItemRefParam)
{
	//I should create a gameplaytags system to let others know if inventory is occupied or not or if itemref/multiitemref is currently occupied or not
	//if item exist in inventory or not, if spesific items exist within itemref/multiitemref or not
	//Also make gameplay tags that singifies if inventory is full or not
	//Gameplay tags for if maininventorywidgeteref exist at all
	//Gameplay tags that writes in the object ID so that I don't have to call anything to see if this should controller should be called when multicasting
	
	if (GetCharacter())
	{
		PawnRef->TakeMasterItem(MasterItemRefParam);
	}
	else
	{
		ICharacterInterface* castingPawn = Cast<ICharacterInterface>(GetCharacter());
		if (castingPawn)
		{
			PawnInterfaceRef = castingPawn;
			PawnInterfaceRef->TakeMasterItem(MasterItemRefParam);
		}
	}

	if (IsValid(MainInventoryWidgetRef))
	{
		if (!bIsInInventory)
		{
			MainInventoryWidgetRef->BuildInventory();
			MainInventoryWidgetRef->BuildGroundItems();
		}

	}

}

void ACPPPlayerController::RefreshInventoryMasterItemDrop(int32 IndexNumParam, const FItemData& ItemDataParam)
{
	if (IsValid(PawnRef))
	{
		PawnRef->RemoveItemFromInventory(IndexNumParam);
		PawnRef->DropMasterItem(ItemDataParam);
		PawnRef->StartMultiTrace();
	}
	else
	{
		ACPPBaseCharacter* castingPawn = Cast<ACPPBaseCharacter>(GetCharacter());
		if (castingPawn)
		{
			PawnRef = castingPawn;
			PawnRef->RemoveItemFromInventory(IndexNumParam);
			PawnRef->DropMasterItem(ItemDataParam);
			PawnRef->StartMultiTrace();
		}
	}

	MainInventoryWidgetRef->BuildInventory();
	MainInventoryWidgetRef->BuildGroundItems();
}

void ACPPPlayerController::ResetPawnRef()
{
	PawnInterfaceRef = nullptr;
}

void ACPPPlayerController::SetPawnInterfaceRef()
{
	PawnInterfaceRef = Cast<ICharacterInterface>(GetCharacter());
}

void ACPPPlayerController::RefreshInventory()
{
	if (IsValid(MainInventoryWidgetRef))
	{
		if (bIsInInventory == false)
		{
			MainInventoryWidgetRef->BuildInventory();
			MainInventoryWidgetRef->BuildGroundItems();
		}
	}
}

void ACPPPlayerController::OpenInventory()
{
	//Do another check for if the player is dead or not
	if (bIsInInventory)
	{
		if (IsValid(MainInventoryWidgetRef))
		{
			if (IsValid(PawnRef))
			{
				PawnRef->StartMultiTrace();
				MainInventoryWidgetRef->SetVisibility(ESlateVisibility::Visible);

				SetShowMouseCursor(true);
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, MainInventoryWidgetRef);
				bIsInInventory = !bIsInInventory;

				MainInventoryWidgetRef->BuildInventory();
				MainInventoryWidgetRef->BuildGroundItems();
			}
			else
			{
				PawnRef = Cast<ACPPBaseCharacter>(GetCharacter());
				PawnRef->StartMultiTrace();

				MainInventoryWidgetRef->SetVisibility(ESlateVisibility::Visible);

				SetShowMouseCursor(true);
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, MainInventoryWidgetRef);
				bIsInInventory = !bIsInInventory;

				MainInventoryWidgetRef->BuildInventory();
				MainInventoryWidgetRef->BuildGroundItems();
			}

		}
		else
		{
			if (IsValid(PawnRef))
			{
				PawnRef->StartMultiTrace();

				MainInventoryWidgetRef = CreateWidget<UCPPMainInvetoryWidget>(this, MainInventoryClassRef);
				MainInventoryWidgetRef->AddToViewport();

				SetShowMouseCursor(true);
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, MainInventoryWidgetRef);
				bIsInInventory = !bIsInInventory;
			}
			else
			{
				PawnRef = Cast<ACPPBaseCharacter>(GetCharacter());
				PawnRef->StartMultiTrace();

				MainInventoryWidgetRef = CreateWidget<UCPPMainInvetoryWidget>(this, MainInventoryClassRef);
				MainInventoryWidgetRef->AddToViewport();

				SetShowMouseCursor(true);
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, MainInventoryWidgetRef);
				bIsInInventory = !bIsInInventory;
			}

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
