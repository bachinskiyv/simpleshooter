// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooter/Pickup.h"
#include "SimpleShooter/Weapons/Weapon.h"

#include "WeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AWeaponPickup : public APickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AWeapon> WeaponClass;
};
