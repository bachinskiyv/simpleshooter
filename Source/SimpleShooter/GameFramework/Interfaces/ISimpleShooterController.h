// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SimpleShooter/ShooterCharacter.h"
#include "UObject/Interface.h"
#include "ISimpleShooterController.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UISimpleShooterController : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPLESHOOTER_API IISimpleShooterController
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Get character class for spawning
	 */
	virtual TSubclassOf<AShooterCharacter> GetCharacterClass() const = 0;

	/**
	 * "Say" Analog
	 */
	virtual void TranslateMessage(const FString& Message) const = 0;

	/**
	 *	Gets player state
	 */
	virtual class AShooterPlayerState* GetShooterPlayerState() const = 0;

	/**
	 * Gets Character
	 */
	virtual AShooterCharacter* GetShooterCharacter() const = 0;
};
