// Fill out your copyright notice in the Description page of Project Settings.


#include "KIllEmAllGameMode.h"
#include "EngineUtils.h"
#include "ShooterAIController.h"
#include "ShooterPlayerController.h"
#include "GameFramework/Controller.h"

void AKIllEmAllGameMode::PawnKilled(APawn* PawnKilled, AController* EventInstigator)
{
	Super::PawnKilled(PawnKilled, EventInstigator);

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	
	if (PlayerController != nullptr)
	{
		EndGame(false);
	}

	for (AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
	{
		if (!AIController->IsDead())
		{
			return;
		}
	}

	EndGame(true);
}

void AKIllEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
