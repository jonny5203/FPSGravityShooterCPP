// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BaseItemInterface.generated.h"

class ACPPBaseCharacter;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBaseItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSGRAVITYSHOOTERCPP_API IBaseItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void EnableWidgetVisibility(ACPPBaseCharacter* BaseCharacterParam) = 0;

	virtual void DisableWidgetVisibility(ACPPBaseCharacter* BaseCharacterParam) = 0;
};
