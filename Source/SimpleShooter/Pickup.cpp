// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->Mobility = EComponentMobility::Static;
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetGenerateOverlapEvents(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Mesh->AddLocalRotation(FRotator(0, 100.f * DeltaTime, 0));
}

bool APickup::CanPickUp()
{
	return bCanPickUp;
}

void APickup::Pickup()
{
	SetActorHiddenInGame(true);
	bCanPickUp = false;

	GetWorld()->GetTimerManager().SetTimer(
		ResetTimerHandle,
		this,
		&APickup::ResetPickup,
		RespawnTime
	);
}

void APickup::ResetPickup()
{
	SetActorHiddenInGame(false);
	bCanPickUp = true;
}

