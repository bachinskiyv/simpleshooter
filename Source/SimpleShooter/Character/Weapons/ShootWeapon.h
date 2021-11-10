// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooter/Weapons/InstantHitWeapon.h"

#include "ShootWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShootWeapon : public AInstantHitWeapon
{
	GENERATED_BODY()

protected:
	virtual void BindWeaponReceivesHitDelegate(UUShotInfo* ShotInfo) override;

	virtual void OnWeaponSystemReceivesHit(const FShotHitInfo* ShotInfo) override;
};
