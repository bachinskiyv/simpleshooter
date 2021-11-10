// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Combat.h"
#include "AIController.h"
#include "SimpleShooter/ShooterCharacter.h"


EBTNodeResult::Type UBTTask_Combat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AShooterCharacter* BotPawn = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (BotPawn != nullptr)
	{
		if (!BotPawn->IsDead())
		{
			Super::ExecuteTask(OwnerComp, NodeMemory);
		}

		return EBTNodeResult::Succeeded; 
	}
	
	return EBTNodeResult::Failed;
}
