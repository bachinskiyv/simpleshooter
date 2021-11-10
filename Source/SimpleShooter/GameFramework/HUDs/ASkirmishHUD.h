// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ASkirmishHUD.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AASkirmishHUD : public AHUD
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void DrawMessage(const FString& Message);
};
