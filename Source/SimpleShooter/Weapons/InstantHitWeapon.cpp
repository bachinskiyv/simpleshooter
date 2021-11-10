// Fill out your copyright notice in the Description page of Project Settings.


#include "InstantHitWeapon.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

bool AInstantHitWeapon::CanApplyDamageForATraceHit(const FHitResult& HitResult)
{
	return HitResult.Actor != nullptr;
}

void AInstantHitWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ShotSystem = Cast<UAbstractShotSystem>(GetComponentByClass(UAbstractShotSystem::StaticClass()));
}

void AInstantHitWeapon::Fire()
{
	FVector StartLocation;
	if (GetMuzzleLocation(StartLocation))
	{
		FVector TargetLocation;
		if (GetShotTarget(TargetLocation))
		{
			if (ShotSystem)
			{
				Super::Fire();
				UUShotInfo* ShotInfo = NewObject<UUShotInfo>();
				ShotInfo->Init(WeaponHolder->GetController(), GetClass());
				BindWeaponReceivesHitDelegate(ShotInfo);

				const FRotator ShotDirection = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
				ShotSystem->MakeAShot(ShotInfo, StartLocation, ShotDirection);
			}
		}
	}
}

bool AInstantHitWeapon::GetShotTarget_Implementation(FVector& Location)
{
	AController* Controller = WeaponHolder->GetController();
	if (Controller == nullptr)
	{
		return false;
	}

	FVector EyesLoc;
	FRotator EyesRot;
	if (Controller->IsPlayerController())
	{
		Controller->GetPlayerViewPoint(EyesLoc, EyesRot);

		FHitResult Hit;
		const bool bResult = GetWorld()->LineTraceSingleByChannel(
			Hit,
			EyesLoc,
			EyesLoc + EyesRot.Vector() * 10000,
			ECC_Camera
		);

		if (bResult && Hit.Actor.IsValid())
		{
			FVector MuzzleLocation;
			GetMuzzleLocation(MuzzleLocation);
			const FVector ShotDirection = Hit.Location - MuzzleLocation;
			const float Dot = FVector::DotProduct(ShotDirection, Hit.Location - EyesLoc);

			if (Dot > 0)
			{
				Location = MuzzleLocation + CalculateShotVectorWithSpread(ShotDirection) * 10000;

				return true;
			}
		}
	}
	else
	{
		Controller->GetActorEyesViewPoint(EyesLoc, EyesRot);	
	}

	// @TODO: distance to parameter
	// Calculate Shot Target
	Location = EyesLoc + CalculateShotVectorWithSpread(EyesRot.Vector()) * 10000;

	return true;
}

FVector AInstantHitWeapon::CalculateShotVectorWithSpread(FVector ShotDirection) const
{
	if (MinSpread == 0 && MaxSpread == 0)
	{
		return ShotDirection;
	}
	
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);

	const float CurrentSpread = FMath::RandRange(MinSpread, MaxSpread);
	const float ConeHalfAngle = FMath::DegreesToRadians(CurrentSpread * .5);
	
	return WeaponRandomStream.VRandCone(ShotDirection, ConeHalfAngle);
}

bool AInstantHitWeapon::GetMuzzleLocation(FVector& Location) const
{
	if (Mesh != nullptr)
	{
		const FTransform MuzzleTransform_WS = Mesh->GetSocketTransform(MuzzleSocketName);

		Location = MuzzleTransform_WS.GetLocation();

		return true;
	}

	return false;
}

void AInstantHitWeapon::BindWeaponReceivesHitDelegate(UUShotInfo* ShotInfo)
{
	ShotInfo->OnShotReceivesHitDelegate.BindUObject(this, &AInstantHitWeapon::OnWeaponSystemReceivesHit);
}

void AInstantHitWeapon::OnWeaponSystemReceivesHit(const FShotHitInfo* ShotInfo)
{
	const bool bDamageApplied = CanApplyDamageForATraceHit(ShotInfo->HitResult);
	if (bDamageApplied)
	{
		const float DamageRange = FMath::RandRange(BaseDamage - DamageShift, BaseDamage + DamageShift);
		const float FinalDamage = FMath::Max(
			0.f,
			DamageRange
		);
		// @TODO: Check that there is no need to inverse normal
		FPointDamageEvent DamageEvent(FinalDamage, ShotInfo->HitResult, ShotInfo->HitResult.Normal, nullptr);
		ShotInfo->HitResult.Actor->TakeDamage(FinalDamage, DamageEvent, ShotInfo->Controller, this);
	}

	OnTraceHitDelegate.Broadcast(ShotInfo->HitResult, bDamageApplied);
}




