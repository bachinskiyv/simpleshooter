// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/ShooterPlayerState.h"
#include "GameFramework/GameModes/FreeForAllGameMode.h"
#include "Kismet/GameplayStatics.h"


AShooterAIController::AShooterAIController()
{
	bWantsPlayerState = true;
	bStopAILogicOnUnposses = true;
}

bool AShooterAIController::IsDead() const
{
	AShooterCharacter* ControlledPawn = Cast<AShooterCharacter>(GetPawn());
	if (ControlledPawn != nullptr)
	{
		return ControlledPawn->IsDead();
	}

	return true;
}

void AShooterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AIBehavior != nullptr && GetPawn() != nullptr)
	{
		FVector PawnLocation = GetPawn()->GetActorLocation();
		RunBehaviorTree(AIBehavior);
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), PawnLocation);
	}
}

void AShooterAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AShooterPlayerState* OurPlayerState = GetShooterPlayerState();
	if (ensure(OurPlayerState != nullptr))
	{
		OurPlayerState->bUseCustomPlayerNames = true;
		if (ensure(!PlayerName.IsEmpty()))
		{
			OurPlayerState->SetPlayerName(PlayerName);	
		}
	}
}

TSubclassOf<AShooterCharacter> AShooterAIController::GetCharacterClass() const
{
	return PawnClass;
}

void AShooterAIController::TranslateMessage(const FString& Message) const
{
	const auto GameMode = GetWorld()->GetAuthGameMode<AFreeForAllGameMode>();
	if (GameMode != nullptr)
	{
		GameMode->SimpleMessageRelayDelegate.Broadcast(Message, ESimpleMessageType::System);
	}
}

AShooterPlayerState* AShooterAIController::GetShooterPlayerState() const
{
	return Cast<AShooterPlayerState>(PlayerState);
}

AShooterCharacter* AShooterAIController::GetShooterCharacter() const
{
	return GetPawn<AShooterCharacter>();
}
