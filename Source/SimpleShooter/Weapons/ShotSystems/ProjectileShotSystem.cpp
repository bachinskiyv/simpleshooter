// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileShotSystem.h"

#include "SimpleShooter/Weapons/Actors/EnrichedProjectileBase.h"

// Sets default values for this component's properties
UProjectileShotSystem::UProjectileShotSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UProjectileShotSystem::MakeAShot(UUShotInfo* ShotInfo, const FVector StartLocation, const FRotator StartRotation)
{
	if (ensure(ProjectileActorClass != nullptr))
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = GetOwner();
		AController* Controller = ShotInfo->GetInstigatorController();
		if (ensure(Controller != nullptr))
		{
			SpawnParameters.Instigator = Controller->GetPawn();
		}
		
		const FVector SpawnLocation = StartLocation + StartRotation.Vector().GetSafeNormal() * StartLocationRelativeOffset;

		AActor* ProjectileActor = GetWorld()->SpawnActor<AActor>(ProjectileActorClass, SpawnLocation, StartRotation, SpawnParameters);
		if (ProjectileActor != nullptr)
		{
			AEnrichedProjectileBase* EnrichedProjectile = Cast<AEnrichedProjectileBase>(ProjectileActor);
			if (EnrichedProjectile != nullptr)
			{
				EnrichedProjectile->SetShotInfo(ShotInfo);
			}
			
			ProjectileActor->OnActorHit.AddDynamic(ShotInfo, &UUShotInfo::OnReceivesHit);
		}
	}
}
