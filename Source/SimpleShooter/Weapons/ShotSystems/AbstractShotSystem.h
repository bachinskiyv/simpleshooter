// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleShooter/Weapons/Objects/UShotInfo.h"

#include "AbstractShotSystem.generated.h"

UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLESHOOTER_API UAbstractShotSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	
	// Sets default values for this component's properties
	UAbstractShotSystem();

	/**
	 * Make a shot
	 */
	virtual void MakeAShot(UUShotInfo* ShotInfo, const FVector StartLocation, const FRotator StartRotation) PURE_VIRTUAL(UAbstractShotSystem::MakeAShot);
};
