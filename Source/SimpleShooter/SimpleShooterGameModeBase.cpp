// Copyright Epic Games, Inc. All Rights Reserved.


#include "SimpleShooterGameModeBase.h"

#include "GameFramework/ShooterGameState.h"
#include "GameFramework/ShooterPlayerState.h"

ASimpleShooterGameModeBase::ASimpleShooterGameModeBase()
{
	PlayerStateClass = AShooterPlayerState::StaticClass();
	GameStateClass = AShooterGameState::StaticClass();
}

void ASimpleShooterGameModeBase::PawnKilled(APawn* PawnKilled, AController* EventInstigator)
{
	if (EventInstigator != nullptr && EventInstigator->PlayerState != nullptr)
	{
		if (PawnKilled->GetController() == EventInstigator)
		{
			EventInstigator->PlayerState->SetScore(EventInstigator->PlayerState->GetScore() - 1);
		}
		else
		{
			EventInstigator->PlayerState->SetScore(EventInstigator->PlayerState->GetScore() + 1);
		}
	}
}
