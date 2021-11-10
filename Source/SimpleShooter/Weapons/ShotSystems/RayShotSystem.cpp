// Fill out your copyright notice in the Description page of Project Settings.


#include "RayShotSystem.h"

#include "DrawDebugHelpers.h"
#include "SimpleShooter/ShooterCharacter.h"

// Sets default values for this component's properties
URayShotSystem::URayShotSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void URayShotSystem::MakeAShot(UUShotInfo* ShotInfo, FVector StartLocation, const FRotator StartRotation)
{
	// Where to we must trace
	const FVector End = StartLocation + StartRotation.Vector() * TraceRange;
	
	DrawDebugLine(
		GetWorld(),
		StartLocation,
		End,
		FColor::MakeRandomColor(),
		false,
		2,
		0,
		1
	);

	AActor* Owner = GetOwner();

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);
	if (Owner)
	{
		// Ignore Weapon owner
		Params.AddIgnoredActor(Owner->GetOwner());
	}
	
	TArray<FHitResult> Hits;
	const bool bHitSuccess = GetWorld()->LineTraceMultiByChannel(
		Hits,
		StartLocation,
		End,
		TraceChannel,
		Params
	);
	
	if (bHitSuccess)
	{
		for (auto Hit : Hits)
		{
			if (Hit.Actor.IsValid())
			{
				ShotInfo->OnReceivesHit(Hit);
			}
		}
	}
}
