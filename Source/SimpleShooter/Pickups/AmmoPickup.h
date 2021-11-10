// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooter/Pickup.h"
#include "SimpleShooter/Weapons/Weapon.h"

#include "AmmoPickup.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AAmmoPickup : public APickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 AmmoAmount = 50;

	/**
	 * Which weapon used by pick up
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AWeapon> WeaponClass;
};
