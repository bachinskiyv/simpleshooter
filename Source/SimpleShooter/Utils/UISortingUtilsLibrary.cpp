// Fill out your copyright notice in the Description page of Project Settings.


#include "UISortingUtilsLibrary.h"


TArray<APlayerState*> UUISortingUtilsLibrary::SortPlayerStatesByScore(TArray<APlayerState*> PlayerArray)
{
	Algo::Sort(PlayerArray, [](const APlayerState* Prev, const APlayerState* Next)
	{
		return Prev->GetScore() > Next->GetScore();
	});

	return PlayerArray;
}

// @TODO: FINISH HER!
FUpperScoresMenuData UUISortingUtilsLibrary::GetFirstAndVariablePosition(TArray<APlayerState*> PlayerArray,  APlayerState* MyPlayerState)
{
	FUpperScoresMenuData ScoresMenuData;
	
	bool bMyPlaceFound = false;
	uint8 Place = 1, Index = 0;
	float LastScore = 0;
	for (auto PlayerState : SortPlayerStatesByScore(PlayerArray))
	{
		Index++;
		if (LastScore < PlayerState->GetScore())
		{
			Place++;
			LastScore = PlayerState->GetScore();
		}
		
		if (PlayerState == MyPlayerState)
		{
			ScoresMenuData.MyPlace = Place;
		}
		
		if (Place > 1)
		{
			
		}
	}

	return ScoresMenuData;
}
