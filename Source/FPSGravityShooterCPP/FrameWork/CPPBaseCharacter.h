// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "FPSGravityShooterCPP/Interfaces/CharacterInterface.h"
#include "FPSGravityShooterCPP/Interfaces/MasterItemInterface.h"
#include "FPSGravityShooterCPP/Interfaces/PlayerControllerInterface.h"
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

	UFUNCTION()
	void MultiSphereTrace();

	UPROPERTY()
	ACPPPlayerController* PlayerControllerRef;

	IPlayerControllerInterface* PCInterfaceRef;

	UFUNCTION(BlueprintCallable)
	void CalledWhenPossessedIsCalled(AController* NewController);

	UPROPERTY(BlueprintReadWrite)
	bool bDead = false;

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
	virtual void StartMultiTrace() override;

	UFUNCTION()
	virtual void EndMultiTrace() override;

private:
	UPROPERTY()
	FTimerHandle MultiSphereTraceTimer;

	class UCameraComponent* CameraComponentRef;

	class USpringArmComponent* SpringArmComponentRef;

	UFUNCTION()
	void LineTrace();

	UFUNCTION(Server, Reliable, WithValidation)
	void DestroyPickupItem(AMasterItem* itemRefParam);
	bool DestroyPickupItem_Validate(AMasterItem* itemRefParam);
	void DestroyPickupItem_Implementation(AMasterItem* itemRefParam);

	IBaseItemInterface* ItemRef;

	TWeakObjectPtr<AActor> ItemActorRef;

	IBoxItemInterface* BoxItemRef;

	TArray<FItemData> Inventory;

	TArray<IMasterItemInterface*> MultiItemRef;

	TArray<AActor*> HitResultArrayToCompare;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerItemAddPawn(AMasterItem* itemRefParam);
	bool ServerItemAddPawn_Validate(AMasterItem* itemRefParam);
	void ServerItemAddPawn_Implementation(AMasterItem* itemRefParam);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerItemRemovePawn(AMasterItem* itemRefParam);
	bool ServerItemRemovePawn_Validate(AMasterItem* itemRefParam);
	void ServerItemRemovePawn_Implementation(AMasterItem* itemRefParam);

	//
	//
	// Just Need To Send a PC reg when multicast as well
	//
	//

	void CheckForGroundTrace(FVector& End, bool& bIsHit, FVector& SpawnVectorParam);

	float CurrentWeight = 0;

	UPROPERTY(EditDefaultsOnly)
	float MaxWeight;

	UFUNCTION()
	bool StackableItemExist(const FText& ItemNameParam, int32& IndexNumParam);

	UFUNCTION()
	void IncreaseAmountOnStackedItem(const FItemData& ItemDataParam, int32& IndexNumParam);

public:
	virtual bool CheckIfBoxCollectionIsNotNull() const override;

	virtual bool GetbDead() const override
	{
		return bDead;
	}

	virtual void SetbDead(bool bDeadParam) override
	{
		this->bDead = bDeadParam;
	}

	virtual IBoxItemInterface* GetBoxItemRef() const override
	{
		return BoxItemRef;
	}

	virtual const float& GetCurrentWeight() const override
	{
		return CurrentWeight;
	}

	void SetCurrentWeight(const float& CurrentWeightParam)
	{
		this->CurrentWeight = CurrentWeightParam;
	}

	virtual const float& GetMaxWeight() const override
	{
		return MaxWeight;
	}

	void SetMaxWeight(const float& MaxWeightParam)
	{
		this->MaxWeight = MaxWeightParam;
	}

	virtual const TArray<IMasterItemInterface*>& GetMultiItemRef() const override
	{
		return MultiItemRef;
	}

	void SetMultiItemRef(const TArray<IMasterItemInterface*>& MultiItemInterfaceRefParam)
	{
		this->MultiItemRef = MultiItemInterfaceRefParam;
	}

	virtual const TArray<FItemData>& GetInventory() const override
	{
		return Inventory;
	}

	void SetInventory(const TArray<FItemData>& InventoryInstance)
	{
		this->Inventory = InventoryInstance;
	}

	UFUNCTION()
	virtual void TakeMasterItem(AMasterItem* MasterItemRefParam) override;

	UFUNCTION()
	bool AddItemToInventory(const FItemData& ItemDataParam);

	UFUNCTION()
	void DestroyMasterItem(AMasterItem* MasterItemParam);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDestroyMasterItem(AMasterItem* MasterItemParam);
	bool ServerDestroyMasterItem_Validate(AMasterItem* MasterItemParam);
	void ServerDestroyMasterItem_Implementation(AMasterItem* MasterItemParam);

	void RebuildInventoryWidget();

	virtual void RemoveItemFromInventory(const int32& IndexNumParam) override;

	virtual void DropMasterItem(const FItemData& ItemDataParam) override;

	virtual void DropBoxCollectionItem(const FItemData& ItemDataParam) override;

	virtual bool ItemExistInMultiRef(IMasterItemInterface* MasterItemInterfaceParam) const override;

	virtual void RemoveFromMultiItemRef(IMasterItemInterface* MasterItemInterfaceParam) override;

	virtual void ReScanMultiItemRef() override;

	virtual void DeleteNonValidReferenceFromMultiItemRef() override;

	float CalculateCurrentWeight();
};
