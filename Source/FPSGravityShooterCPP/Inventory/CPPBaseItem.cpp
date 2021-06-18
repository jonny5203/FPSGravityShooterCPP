// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPBaseItem.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FPSGravityShooterCPP/Interfaces/CharacterInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "FPSGravityShooterCPP/FrameWork/CPPBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "FPSGravityShooterCPP/FrameWork/CPPPlayerController.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACPPBaseItem::ACPPBaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName(TEXT("PickupCollision"));
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(true);
	Mesh->CanCharacterStepUpOn = ECB_No;
	RootComponent = Mesh;

	static ConstructorHelpers::FClassFinder<UUserWidget> PickupWidgetObj(TEXT("/Game/FPS/UI/Inventory/WB_PickupTest"));
	if (PickupWidgetObj.Succeeded())
	{
		Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
		Widget->SetWidgetClass(PickupWidgetObj.Class);
		Widget->SetVisibility(false);
		Widget->SetWidgetSpace(EWidgetSpace::Screen);
		Widget->SetCollisionProfileName(TEXT("UI"));
		Widget->SetupAttachment(Mesh);
	}
}

// Called when the game starts or when spawned
void ACPPBaseItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACPPBaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPPBaseItem::EnableWidgetVisibility(ACPPBaseCharacter* PawnRef)
{
	if (PawnRef)
	{
		if (PawnRef->IsLocallyControlled())
		{
			SetWidgetVisibility();
			GetWorld()->GetTimerManager().SetTimer(WidgetRotationTimerHandle, this,
			                                       &ACPPBaseItem::SetWidgetRotationTimer, 0.2, true);
		}
	}
}

void ACPPBaseItem::DisableWidgetVisibility(ACPPBaseCharacter* PawnRef)
{
	if (PawnRef)
	{
		if (PawnRef->IsLocallyControlled())
		{
			Widget->SetVisibility(false);
			GetWorld()->GetTimerManager().ClearTimer(WidgetRotationTimerHandle);
		}
	}
}

bool ACPPBaseItem::ServerAddPawnRef_Validate(ACPPPlayerController* PCRefParam)
{
	if (IsValid(PCRefParam))
	{
		return true;
	}
	return false;
}

void ACPPBaseItem::ServerAddPawnRef_Implementation(ACPPPlayerController* PCRefParam)
{
	if (HasAuthority())
	{
		PCRefList.AddUnique(PCRefParam);
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black,
			                                 FString::FromInt(PCRefList.Num()) + " What the Fuck is this");
		}
	}
}

bool ACPPBaseItem::ServerRemovePawnRef_Validate(ACPPPlayerController* PCRefParam)
{
	if (IsValid(PCRefParam))
	{
		return true;
	}
	return false;
}

void ACPPBaseItem::ServerRemovePawnRef_Implementation(ACPPPlayerController* PCRefParam)
{
	PCRefList.Remove(PCRefParam);
}

void ACPPBaseItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPPBaseItem, PCRefList);
}

void ACPPBaseItem::SetWidgetVisibility()
{
	Widget->SetVisibility(true);
}

void ACPPBaseItem::SetWidgetRotationTimer()
{
	FVector actorLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetCapsuleComponent()->
		GetComponentLocation();
	FVector widgetLocation = Widget->GetComponentLocation();

	FRotator lookUpRot = UKismetMathLibrary::FindLookAtRotation(widgetLocation, actorLocation);
	Widget->SetWorldRotation(lookUpRot);
}

void ACPPBaseItem::RefreshList_Implementation()
{
	for (AController* PC : PCRefList)
	{
		ICharacterInterface* characterInterface = Cast<ICharacterInterface>(PC);
		if (characterInterface)
		{
			characterInterface->RefreshInventoryInterface();
		}
	}
}
