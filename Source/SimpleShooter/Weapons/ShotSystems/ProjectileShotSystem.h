// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbstractShotSystem.h"
#include "SimpleShooter/Weapons/Objects/UShotInfo.h"

#include "ProjectileShotSystem.generated.h"

/**
* Weapon component for shooting projectiles
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLESHOOTER_API UProjectileShotSystem : public UAbstractShotSystem
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProjectileShotSystem();

	virtual void MakeAShot(UUShotInfo* ShotInfo, const FVector StartLocation, const FRotator StartRotation) override;

protected:
	
	/**
	 * Projectile actor, which spawned
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=Config)
	TSubclassOf<AActor> ProjectileActorClass;

	/**
	 * Relative offset for start location (example: for comfort projectiles spawn)
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=Config)
	float StartLocationRelativeOffset = 0;
};
