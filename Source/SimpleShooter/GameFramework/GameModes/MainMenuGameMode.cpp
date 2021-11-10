// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "SimpleShooter/ShooterAIController.h"
#include "SimpleShooter/GameFramework/ShooterPlayerState.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	bUseSeamlessTravel = true;
}

void AMainMenuGameMode::GetSeamlessTravelActorList(bool bToTransition, TArray<AActor*>& ActorList)
{
	Super::GetSeamlessTravelActorList(bToTransition, ActorList);
	
	TArray<AActor*> AIControllers;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AShooterAIController::StaticClass(),
		AIControllers
	);

	ActorList.Append(AIControllers);

	for (AActor* Controller : AIControllers)
	{
		const AShooterAIController* AIController = Cast<AShooterAIController>(Controller);
		if (AIController != nullptr)
		{
			AShooterPlayerState* TransferredPlayerState = AIController->GetPlayerState<AShooterPlayerState>();
			if (TransferredPlayerState != nullptr)
			{
				ActorList.Add(TransferredPlayerState);
			}
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("ACTOLIST LENGTH: %d"), ActorList.Num());
}
