// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SimpleShooter/Weapons/Objects/UShotInfo.h"
#include "Misc/Guid.h"
#include "SimpleShooter/ShooterPlayerController.h"

#include "ShooterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerState : public APlayerState
{
	GENERATED_BODY()

	AShooterPlayerState();

public:
	void CollectVictimHit(const FShotHitInfo ShotHitInfo);

	void CollectVictimDeath(const IISimpleShooterController* KillerController, const IISimpleShooterController* VictimController, const FShotHitInfo DeadHitInfo, FDamageEvent DamageEvent);

	UFUNCTION(BlueprintImplementableEvent)
	void DetermineKillText(const FString& KillerName, const FString& VictimName, TSubclassOf<AWeapon> WeaponClass, TSubclassOf<UDamageType> DamageTypeClass, FString& OutString);
	
protected:
	bool bLastRailHitsCharacter = false;

	FGuid LastRocketShotGuid;

	FTimerHandle ExcellentKillTimerHandle;

	TSubclassOf<AWeapon> RailClass;

	const float ExcellentKillTimeout = 3.f;

	void CheckAndRerunExcellentChallenge();
};
