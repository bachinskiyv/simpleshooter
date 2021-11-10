// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootWeapon.h"

#include "SimpleShooter/ShooterCharacter.h"
#include "SimpleShooter/Character/Damage/DamageTypes.h"

void AShootWeapon::BindWeaponReceivesHitDelegate(UUShotInfo* ShotInfo)
{
	ShotInfo->OnShotReceivesHitDelegate.BindUObject(this, &AShootWeapon::OnWeaponSystemReceivesHit);
}

void AShootWeapon::OnWeaponSystemReceivesHit(const FShotHitInfo* ShotInfo)
{
	const bool bDamageApplied = CanApplyDamageForATraceHit(ShotInfo->HitResult);
	if (bDamageApplied)
	{
		const float DamageRange = FMath::RandRange(BaseDamage - DamageShift, BaseDamage + DamageShift);
		float FinalDamage = FMath::Max(
			0.f,
			DamageRange
		);

		FPointDamageEvent DamageEvent(FinalDamage, ShotInfo->HitResult, ShotInfo->HitResult.Normal, nullptr);
		AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(ShotInfo->HitResult.Actor);
		if (ShooterCharacter != nullptr)
		{
			ShooterCharacter->TakeDamage(*ShotInfo, FinalDamage, DamageEvent, this);
		}
	}

	OnTraceHitDelegate.Broadcast(ShotInfo->HitResult, bDamageApplied);
}
