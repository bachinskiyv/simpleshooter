// Fill out your copyright notice in the Description page of Project Settings.


#include "FreeForAllGameMode.h"

#include "EngineUtils.h"
#include "SimpleShooter/ShooterAIController.h"
#include "SimpleShooter/GameFramework/Interfaces/ISimpleShooterController.h"

void AFreeForAllGameMode::BeginPlay()
{
	Super::BeginPlay();

	bool bHasBots = false;
	for (AShooterAIController* ShooterAIController : TActorRange<AShooterAIController>(GetWorld()))
	{
		bHasBots = true;
		RestartPlayer(ShooterAIController);
	}

	if (!bHasBots)
	{
		SpawnBots();
	}
}

void AFreeForAllGameMode::PawnKilled(APawn* PawnKilled, AController* EventInstigator)
{
	Super::PawnKilled(PawnKilled, EventInstigator);

	AController* PlayerController = PawnKilled->GetController();
	if (PlayerController != nullptr)
	{
		PlayerController->UnPossess();

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [PawnKilled, PlayerController, this]()
		{
			PawnKilled->Destroy();
			RestartPlayer(PlayerController);
		}, 5.f, false);
	}
}

UClass* AFreeForAllGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	IISimpleShooterController* SuitableController = Cast<IISimpleShooterController>(InController);
	if (SuitableController != nullptr)
	{
		return SuitableController->GetCharacterClass();
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AFreeForAllGameMode::SpawnBots()
{
	const uint8 NumSpawnBots = 1;

	UWorld* World = GetWorld();
	for (uint8 Index = 0; Index < NumSpawnBots; ++Index)
	{
		AAIController* BotController = World->SpawnActor<AAIController>(AIControllerClass);
		RestartPlayer(BotController);
	}
}

