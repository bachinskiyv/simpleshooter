// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbstractShotSystem.h"
#include "SimpleShooter/Weapons/Objects/UShotInfo.h"

#include "RayShotSystem.generated.h"


/**
 * Weapon component for shooting rays
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLESHOOTER_API URayShotSystem : public UAbstractShotSystem
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URayShotSystem();

	virtual void MakeAShot(UUShotInfo* ShotInfo, const FVector StartLocation, const FRotator StartRotation) override;

protected:
	/**
	 * Shooting Trace Range
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float TraceRange = 100000.f;

	/**
	 * Collision channel for weapon
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Pawn;
};
