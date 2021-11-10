// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LastExaminedLocations.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ULastExaminedLocations : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Push element onto end of queue
	 */
	void Push(const FVector& Location);

	/**
	 * Gets Diff with passed POI locations
	 */
	TArray<FVector> GetDiff(TArray<FVector> POILocations);
	
protected:
	TArray<FVector> ExaminedLocations;
};
