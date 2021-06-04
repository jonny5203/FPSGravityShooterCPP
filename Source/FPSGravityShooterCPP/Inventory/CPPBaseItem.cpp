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
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACPPBaseItem::ACPPBaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(250.0f);
	RootComponent = Collision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);

	static ConstructorHelpers::FClassFinder<UUserWidget> PickupWidgetObj(TEXT("/Game/FPS/UI/Inventory/WB_PickupTest"));
	if (PickupWidgetObj.Succeeded())
	{
		Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
		Widget->SetWidgetClass(PickupWidgetObj.Class);
		Widget->SetVisibility(false);
		Widget->SetWidgetSpace(EWidgetSpace::Screen);
		Widget->SetupAttachment(Collision);
	}

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACPPBaseItem::OnSphereBeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ACPPBaseItem::OnSphereEndOverlap);

	
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

void ACPPBaseItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UCharacterInterface::StaticClass()))
	{
		ACPPBaseCharacter* pawnObj = Cast<ACPPBaseCharacter>(OtherActor);
		if (pawnObj)
		{
			PawnRefList.Add(pawnObj);
			if (pawnObj->IsLocallyControlled())
			{
				SetWidgetVisibility();
				GetWorld()->GetTimerManager().SetTimer(WidgetRotationTimerHandle, this,
				                                       &ACPPBaseItem::SetWidgetRotationTimer, 0.2, true);
			}
		}
	}
}

void ACPPBaseItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPPBaseCharacter* pawnObj = Cast<ACPPBaseCharacter>(OtherActor);
	if (pawnObj)
	{
		PawnRefList.Remove(pawnObj);

		if (pawnObj->IsLocallyControlled())
		{
			Widget->SetVisibility(false);
			GetWorld()->GetTimerManager().ClearTimer(WidgetRotationTimerHandle);
		}
	}
}

void ACPPBaseItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPPBaseItem, PawnRefList);
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
	for (AActor* PawnActor : PawnRefList)
	{
		ICharacterInterface* characterInterface = Cast<ICharacterInterface>(PawnActor);
		if (characterInterface)
		{
			characterInterface->RefreshInventory();
		}
	}
}
