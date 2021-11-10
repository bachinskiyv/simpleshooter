// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFramework/PlayerState.h"
#include "UISortingUtilsLibrary.generated.h"

USTRUCT(BlueprintType)
struct FUpperScoresMenuData
{
	GENERATED_BODY()

	float FirstPlaceScore;
	float VariablePlaceScore;
	uint8 MyPlace;
};

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UUISortingUtilsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Sorts default player states by score
	 */
	UFUNCTION(BlueprintCallable, Category="UI|Sort")
	static TArray<APlayerState*> SortPlayerStatesByScore(TArray<APlayerState*> PlayerArray);

	/**
	 * !INCOMPLETED! Gets First position score, and second if you have first or second place, otherwise gets yours 
	 */
	UE_DEPRECATED(4.20, "INCOMPLETED")
	static FUpperScoresMenuData GetFirstAndVariablePosition(TArray<APlayerState*> PlayerArray, APlayerState* MyPlayerState);
};
