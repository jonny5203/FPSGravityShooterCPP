// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
#include "CPPBaseItem.generated.h"

class ACPPPlayerController;
UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	None UMETA(DisplayName = "None"),
	Consumeables UMETA(DisplayName = "Consumeables"),
	Collectibles UMETA(DisplayName = "Collectibles"),
	Weapon UMETA(DisplayName = "Weapon"),
	Ammo UMETA(DisplayName = "Ammo"),
	Abilities UMETA(DisplayName = "Abilities")
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	None UMETA(DisplayName = "None"),
	Common UMETA(DisplayName = "Common"),
	Rare UMETA(DisplayName = "Rare"),
	Legendary UMETA(DisplayName = "Legendary"),
	Epic UMETA(DisplayName = "Epic"),
	GodLevel UMETA(DisplayName = "GodLevel")
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText Name;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UTexture2D* Image;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float Weight;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	EItemCategory Category;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 Amount;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bIsStackable;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<class AMasterItem> ClassOfItem;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	EItemRarity ItemRarity;

	friend bool operator==(const FItemData& A, const FItemData& B)
	{
		return A.Name.EqualTo(B.Name, ETextComparisonLevel::Default);
	}

	void IncreaseAmount(int32 amountToAdd)
	{
		Amount = Amount + amountToAdd;
	}
};

class ACPPBaseCharacter;

UCLASS(BlueprintType)
class FPSGRAVITYSHOOTERCPP_API ACPPBaseItem : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACPPBaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FGameplayTag CharacterTag;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class UWidgetComponent* Widget;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override
	{
		TagContainer = GameplayTags;
		return;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
	FGameplayTagContainer GameplayTags;

	/** called when something enters the sphere component */
	UFUNCTION()
	void DisableWidgetVisibility(ACPPBaseCharacter* PawnRef);

	/** called when something leaves the sphere component */
	UFUNCTION()
	void EnableWidgetVisibility(ACPPBaseCharacter* PawnRef);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAddPawnRef(ACPPPlayerController* PCRefParam);
	bool ServerAddPawnRef_Validate(ACPPPlayerController* PCRefParam);
	void ServerAddPawnRef_Implementation(ACPPPlayerController* PCRefParam);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRemovePawnRef(ACPPPlayerController* PCRefParam);
	bool ServerRemovePawnRef_Validate(ACPPPlayerController* PCRefParam);
	void ServerRemovePawnRef_Implementation(ACPPPlayerController* PCRefParam);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool bActivatedWidget;

	UFUNCTION()
	void SetWidgetVisibility();

	UFUNCTION()
	void SetWidgetRotationTimer();

	UPROPERTY()
	FTimerHandle WidgetRotationTimerHandle;

	UPROPERTY(Replicated)
	TArray<ACPPPlayerController*> PCRefList;

	UFUNCTION(Server, Reliable)
	virtual void RefreshList();
	virtual void RefreshList_Implementation();
};
