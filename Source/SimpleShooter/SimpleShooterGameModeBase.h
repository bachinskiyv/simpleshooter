// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/Widget.h"
#include "GameFramework/GameMode.h"
#include "SimpleShooterGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ASimpleShooterGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	ASimpleShooterGameModeBase();
	
	virtual void PawnKilled(APawn* PawnKilled, AController* Instigator);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Classes)
	TSubclassOf<UWidget> ScoresWidgetClass;
};
