// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooter/SimpleShooterGameModeBase.h"
#include "AIController.h"
#include "FreeForAllGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSimpleMessageRelaySignature, const FString&, ESimpleMessageType);

UENUM(BlueprintType, DisplayName="Message Type")
enum class ESimpleMessageType : uint8
{
	System UMETA(DisplayName="System"),
	Chat UMETA(DisplayName="Chat")
};

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AFreeForAllGameMode : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()

public:
	FOnSimpleMessageRelaySignature SimpleMessageRelayDelegate;
	
	virtual void BeginPlay() override;

	virtual void PawnKilled(APawn* PawnKilled, AController* Instigator) override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
protected:
	UPROPERTY(EditDefaultsOnly, Category="Bots")
	TSubclassOf<AAIController> AIControllerClass;
	
	void SpawnBots();
};
