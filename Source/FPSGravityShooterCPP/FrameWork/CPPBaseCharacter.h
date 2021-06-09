// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "FPSGravityShooterCPP/Interfaces/CharacterInterface.h"
#include "FPSGravityShooterCPP/Inventory/BoxCollectionItem.h"
#include "FPSGravityShooterCPP/Inventory/MasterItem.h"
#include "CPPBaseCharacter.generated.h"

class ACPPPlayerController;
class UCPPMainInvetoryWidget;

UCLASS(BlueprintType)
class FPSGRAVITYSHOOTERCPP_API ACPPBaseCharacter : public ACharacter, public IGameplayTagAssetInterface,
                                                   public ICharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPPBaseCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTag ItemClassTag;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTag WeaponTag;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTag AmmoTag;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTag BoxCollection;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTag Consumeables;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTag Collectibles;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTag Abilities;

	UFUNCTION(BlueprintCallable)
	void PickupLineTrace();

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* PickupWidget;

	UFUNCTION(BlueprintCallable)
	void AddToInventory();

	UFUNCTION(BlueprintCallable)
	void MultiSphreTrace();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override
	{
		TagContainer = GameplayTags;
		return;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
	FGameplayTagContainer GameplayTags;

	UFUNCTION()
	virtual void RefreshInventoryInterface() override;

	UFUNCTION()
	virtual void StartMultiTrace() override;

	UFUNCTION()
	virtual void EndMultiTrace() override;

private:
	class UCameraComponent* CameraComponentRef;

	class USpringArmComponent* SpringArmComponentRef;

	UFUNCTION()
	void LineTrace();

	UFUNCTION(Server, Reliable, WithValidation)
	void DestroyPickupItem(AMasterItem* itemRefParam);
	bool DestroyPickupItem_Validate(AMasterItem* itemRefParam);
	void DestroyPickupItem_Implementation(AMasterItem* itemRefParam);

	AMasterItem* ItemRef;

	ABoxCollectionItem* BoxItemRef;

	TArray<FItemData> Inventory;

	TArray<AMasterItem*> MultiItemRef;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerItemAddPawn(AMasterItem* itemRefParam, AController* PCRefParam);
	bool ServerItemAddPawn_Validate(AMasterItem* itemRefParam, AController* PCRefParam);
	void ServerItemAddPawn_Implementation(AMasterItem* itemRefParam, AController* PCRefParam);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerItemRemovePawn(AMasterItem* itemRefParam);
	bool ServerItemRemovePawn_Validate(AMasterItem* itemRefParam);
	void ServerItemRemovePawn_Implementation(AMasterItem* itemRefParam);

public:
	TArray<AMasterItem*> GetMultiItemRef() const
	{
		return MultiItemRef;
	}

	void SetMultiItemRef(const TArray<AMasterItem*>& MultiItemRefParam)
	{
		this->MultiItemRef = MultiItemRefParam;
	}

	const TArray<FItemData>& GetInventory() const
	{
		return Inventory;
	}

	void SetInventory(const TArray<FItemData>& InventoryInstance)
	{
		this->Inventory = InventoryInstance;
	}

	UFUNCTION()
	void AddItemToInventory(const FItemData& ItemDataParam);

	UFUNCTION()
	void DestroyMasterItem(AMasterItem* MasterItemParam);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDestroyMasterItem(AMasterItem* MasterItemParam);
	bool ServerDestroyMasterItem_Validate(AMasterItem* MasterItemParam);
	void ServerDestroyMasterItem_Implementation(AMasterItem* MasterItemParam);

	void RebuildInventoryWidget();
};
