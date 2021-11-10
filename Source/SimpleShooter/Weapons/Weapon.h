// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IPawnHoldsWeapon.h"
#include "Engine/World.h"

#include "Weapon.generated.h"

/**
 * When Weapon Fires
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireDelegateSignature);

/**
 * Describes Weapon fire modes
 */
UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	OneShot,

	Burst,

	FullAuto
};

UENUM()
enum class EWeaponState
{
	Idle,
	Reloading,
	SwitchFireMode,
	Firing
};

/**
 *	Cooldown system for weapon
 */
USTRUCT()
struct FWeaponShootingCooldown
{
	GENERATED_BODY()

	FWeaponShootingCooldown()
		: bTimerIsActive(false)
	{
		TimerDelegate.BindLambda([this]()
		{
			bTimerIsActive = false;
		});
	}

	void SetTimerIfAble(UWorld* WorldObjectContext, float CooldownDelay)
	{
		if (!bTimerIsActive)
		{
			WorldObjectContext->GetTimerManager().SetTimer(
				TimerHandle,
				TimerDelegate,
				CooldownDelay,
				false
			);

			bTimerIsActive = true;
		}
	}

	bool GetTimerIsActive() const
	{
		return bTimerIsActive;
	}

	void ResetTimer(UWorld* WorldObjectContext)
	{
		WorldObjectContext->GetTimerManager().ClearTimer(TimerHandle);
		bTimerIsActive = false;
	}

protected:

	/**
	 * Prevent shooting faster than cooldown
	 *
	 * @TODO: How it must work with oneshot?
	 */
	FTimerHandle TimerHandle;

private:
	bool bTimerIsActive;

	FTimerDelegate TimerDelegate;
};


/**
 * Owner of AWeapon must realize {IPawnHoldsWeapon} interface
 */
UCLASS(Abstract, Blueprintable)
class SIMPLESHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnFireDelegateSignature OnFireDelegate;
	
	// Sets default values for this actor's properties
	AWeapon();

	/**
	 * Start shooting
	 */
	UFUNCTION(BlueprintCallable)
	void StartShooting();

	/**
	 * EndShooting
	 */
	UFUNCTION(BlueprintCallable)
	void StopShooting();

	/**
	 * Check owner compatibility on SetOwner
	 */
	virtual void SetOwner(AActor* NewOwner) override;

	/**
	 *
	 */
	virtual void Tick(float DeltaSeconds) override;

	/**
	 * Gets current ammo count
	 */
	int32 GetCurrentAmmo() const;

	/**
	* Represents rules for ammo equipment
	*
	* @return Took ammo?
	*/
	UFUNCTION(BlueprintNativeEvent)
	bool PickAmmo(int32 Amount);

	/**
	 * Getting Mesh pointer
	 */
	class USkeletalMeshComponent* GetMesh() const;

	/**
	 * Getting initial ammo amount
	 */
	int32 GetInitialAmmoAmount() const;

protected:
	
	/**
	 * Weapon mesh
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Components)
	class USkeletalMeshComponent* Mesh;

	/**
	 * Maximum weapon ammo
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Config)
	int32 MaxAmmo = 200;

	/**
	 * How much ammo weapon consumes per shot
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Config)
	int32 ConsumeAmmoPerShot = 1;

	/**
	 * Weapon starts ammo
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Config)
	int32 InitialAmmo = 50;

	/**
	 * Base attack damage
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Config)
	float BaseDamage = 10.f;

	/**
	 * Random Damage shift -half<BaseDame>+half
	 */
	UPROPERTY(EditDefaultsOnly, Category=Config)
	float DamageShift = 5.f;
	
	/**
	 * Fire rate
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Config)
	float TimeBetweenShoots = 0.5;

	/**
	 * Fire mode
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Config)
	EWeaponFireMode FireMode = EWeaponFireMode::FullAuto;

	/**
	 * If FireMode is set to {EWeaponFireMode::Burst} it will limit shots per row
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Config)
	int32 BurstCount = 3;

	/**
	 * If we can restart burst after break in shooting, in time of one RequestShooting if FireMode set to {EWeaponFireMode::Burst}
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Config)
	bool bCanRestartBurstShooting = false;

	/**
	 * Does it is not consumes ammo?
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Config)
	bool bHasInfiniteAmmo = false;

	/**
	* Current ammo in clip
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	int32 CurrentAmmo;

	/**
	 * Will communicate through interface
	 */
	IIPawnHoldsWeapon* WeaponHolder;

	/**
	 * Fire!
	 */
	virtual void Fire();

	/**
	 * Fire Request
	 */
	void MakeFireRequest();

	/**
	 * Does Weapon can make a shoot?
	 */
	bool CanShoot() const;

	/**
	 * Consume ammo for the weapon
	 */
	void ConsumeAmmo();

private:
	/**
	 * Shooting cooldown structure
	 */
	FWeaponShootingCooldown ShootingCooldown;
	
	/**
	 * Burst or FullAuto shooting
	 */
	FTimerHandle ShootTimerHandle;

	/**
	 * Remaining bullets in burst row
	 */
	int32 BurstRemaining = 0;

	/**
	* If we request shooting - pressed key
	*/
	bool bRequestShooting = false;

	/**
	 * If we can't shoot at some time
	 */
	void RestartBurstShooting();

	/**
	 * Stop the burst row
	 */
	void StopBurstShooting();
};
