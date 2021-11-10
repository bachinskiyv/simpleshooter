// Fill out your copyright notice in the Description page of Project Settings.


#include "LastExaminedLocations.h"

void ULastExaminedLocations::Push(const FVector& Location)
{
	ExaminedLocations.Remove(Location);
	ExaminedLocations.Add(Location);
}

TArray<FVector> ULastExaminedLocations::GetDiff(TArray<FVector> POILocations)
{
	TArray<FVector> DiffArray;
	const int32 Size = FMath::Max(0, POILocations.Num() - ExaminedLocations.Num());
	DiffArray.Reserve(Size);

	if (Size == 0)
	{
		return DiffArray;
	}

	for (const FVector& Location : POILocations)
	{
		if (!ExaminedLocations.Contains(Location))
		{
			DiffArray.Add(Location);
		}
	}

	return DiffArray;
}


