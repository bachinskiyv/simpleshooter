// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleShooter/Weapons/Objects/UShotInfo.h"

#include "EnrichedProjectileBase.generated.h"

UCLASS(Blueprintable, Abstract)
class SIMPLESHOOTER_API AEnrichedProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnrichedProjectileBase();

	/**
	 * Sets ShotInfo data
	 */
	virtual void SetShotInfo(UUShotInfo* ShotInfo);

	/**
	 * Gets ShotInfo data
	 */
	virtual UUShotInfo* GetShotInfo();

private:
	UPROPERTY()
	UUShotInfo* ShotInfo;
};
