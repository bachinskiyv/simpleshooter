// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetNextPOILocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetNextPOILocation::UBTTask_GetNextPOILocation()
{
	NodeName = TEXT("Get Next POI Location");
}

EBTNodeResult::Type UBTTask_GetNextPOILocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (SuperResult == EBTNodeResult::Succeeded && GetWorld())
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		APawn* BotPawn = OwnerComp.GetAIOwner()->GetPawn();
		
		if (NavSystem != nullptr && BotPawn != nullptr)
		{
			FNavLocation NavLocation;
			if (NavSystem->GetRandomReachablePointInRadius(BotPawn->GetActorLocation(), NavigableRadius,NavLocation))
			{
				UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
				BlackboardComponent->SetValueAsVector(BlackboardKey.SelectedKeyName, NavLocation.Location);
				
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
