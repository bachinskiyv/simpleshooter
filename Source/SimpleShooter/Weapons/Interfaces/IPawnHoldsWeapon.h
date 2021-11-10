// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPawnHoldsWeapon.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIPawnHoldsWeapon : public UInterface
{
	GENERATED_BODY()
};

/**
 * Pawn With Holding weapon interface
 */
class SIMPLESHOOTER_API IIPawnHoldsWeapon
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CanShoot();

	virtual AController* GetController() const = 0;
};
