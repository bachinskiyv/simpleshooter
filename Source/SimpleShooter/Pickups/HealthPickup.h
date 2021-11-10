// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooter/Pickup.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AHealthPickup : public APickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float HealthAmount = 0;
};
