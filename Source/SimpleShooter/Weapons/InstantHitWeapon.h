// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Weapon.h"
#include "Objects/UShotInfo.h"
#include "ShotSystems/AbstractShotSystem.h"

#include "InstantHitWeapon.generated.h"

/**
* Weapon Trace hit
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTraceHitSignature, const FHitResult, HitResult, bool, bDamageWasApplied);

USTRUCT(BlueprintType)
struct FWeaponSpreadSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinSpread = 2.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpread = 5.f;

	// @TODO: FIRST SHOOT IN A ROW
	float FirstShootSpread = 0.f;
};

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AInstantHitWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnTraceHitSignature OnTraceHitDelegate;

	/**
	 * Muzzle Socket Name
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName MuzzleSocketName = TEXT("Muzzle");
	
	/**
	* If we can apply damage for a current trace hit
	*/
	virtual bool CanApplyDamageForATraceHit(const FHitResult& HitResult);

	/**
	 * Initialize ShotSystem
	 */
	virtual void PostInitializeComponents() override;
	
protected:
	/**
	 * Shot System for this weapon
	 */
	UPROPERTY()
	UAbstractShotSystem* ShotSystem;

	/**
	 * Make a shot
	 */
	UFUNCTION()
	virtual void Fire() override;

	/**
	 * Gets shoot target location 
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool GetShotTarget(FVector& Location);

	/**
	 * Minimum weapon spread
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config | Spread")
	float MinSpread = 2.f;

	/**
	* Maximum weapon spread
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config | Spread")
	float MaxSpread = 5.f;

	/**
	 * Calculate shot direction with spread
	 */
	FVector CalculateShotVectorWithSpread(FVector ShotDirection) const;

	/**
	 * Gets Muzzle Location for the shoot
	 */
	bool GetMuzzleLocation(FVector& Location) const;

	/**
	 *	Bind ShotInfo hits delegate
	 */
	virtual void BindWeaponReceivesHitDelegate(UUShotInfo* ShotInfo);

	/**
	 * When weapon system receives hit
	 */
	virtual void OnWeaponSystemReceivesHit(const FShotHitInfo* ShotInfo);
};
