// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
	RootComponent = Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	// No need receive decals on weapon
	Mesh->bReceivesDecals = false;
	// No need collision
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// No need traces
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	/**
	 * Picks up the initial ammo
	 */
	CurrentAmmo = InitialAmmo;
}

void AWeapon::StartShooting()
{
	bRequestShooting = true;

	switch (FireMode)
	{
		case EWeaponFireMode::OneShot:
			MakeFireRequest();
		break;

		case EWeaponFireMode::Burst:
			BurstRemaining = BurstCount;
		/* No Break! */
		case EWeaponFireMode::FullAuto:
			RestartBurstShooting();
	}
}

void AWeapon::StopShooting()
{
	bRequestShooting = false;

	StopBurstShooting();
}

void AWeapon::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);

	/**
	 * @TODO: Dont use get owner?
	 */
	if (GetOwner() != nullptr)
	{
		WeaponHolder = Cast<IIPawnHoldsWeapon>(GetOwner());

		check(WeaponHolder != nullptr);
	}
}

void AWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// If we must shooting
	if (bRequestShooting)
	{
		if (CanShoot())
		{
			if (
				!ShootTimerHandle.IsValid() && FireMode == EWeaponFireMode::FullAuto ||
				(FireMode == EWeaponFireMode::Burst && BurstRemaining > 0)
			)
			{
				if (FireMode == EWeaponFireMode::Burst)
				{
					BurstRemaining = BurstCount;
				}
				
				RestartBurstShooting();
			}
		}
		else
		{
			// Stopping burst shooting or stopping ammo consuming
			StopBurstShooting();
		}
	}
}

/**
 * @TODO: ADD COOLDOWN TO NEXT SHOT
 */ 

int32 AWeapon::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

USkeletalMeshComponent* AWeapon::GetMesh() const
{
	return Mesh;
}

int32 AWeapon::GetInitialAmmoAmount() const
{
	return InitialAmmo;
}

void AWeapon::RestartBurstShooting()
{
	if (!ShootingCooldown.GetTimerIsActive()) {
		GetWorldTimerManager().SetTimer(
			ShootTimerHandle,
			this,
			&AWeapon::MakeFireRequest,
			TimeBetweenShoots,
			true,
			0
		);

		ShootingCooldown.SetTimerIfAble(GetWorld(), TimeBetweenShoots);
	}
}

void AWeapon::StopBurstShooting()
{
	GetWorldTimerManager().ClearTimer(ShootTimerHandle);
}

bool AWeapon::PickAmmo_Implementation(int32 Amount)
{
	if (MaxAmmo > CurrentAmmo)
	{
		CurrentAmmo = FMath::Min(MaxAmmo, CurrentAmmo + Amount);

		return true;
	}

	return false;
}

void AWeapon::Fire()
{
	OnFireDelegate.Broadcast();
	ConsumeAmmo();
}

void AWeapon::MakeFireRequest()
{
	if (FireMode == EWeaponFireMode::Burst)
	{
		--BurstRemaining;
	}
	
	Fire();
}

void AWeapon::ConsumeAmmo()
{
	CurrentAmmo -= ConsumeAmmoPerShot;
}

bool AWeapon::CanShoot() const
{
	return bRequestShooting && WeaponHolder->CanShoot() &&
		(CurrentAmmo >= ConsumeAmmoPerShot || bHasInfiniteAmmo);
}

