// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleShooter/Weapons/Weapon.h"
#include "WeaponsInventoryComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnEquipCompleted, AWeapon*);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipBegin, TSubclassOf<AWeapon>, WeaponClass);

/**
 * Weapons container with settings
 */
USTRUCT(BlueprintType)
struct FWeaponContainer
{
	GENERATED_BODY()

	/**
	 * Weapon item class
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AWeapon> WeaponClass;

	/**
	 * Does this weapon must be equipped at start
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bIsEquippedAtStart;
	
	/**
	* Does this weapon must be picked up at start
	*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	bool bPickedUpAtStart;

	FWeaponContainer()
		: bIsEquippedAtStart(false), bPickedUpAtStart(false), Weapon(nullptr)
	{}

	/**
	 * Gets weapon pointer
	 */
	AWeapon* GetWeapon() const
	{
		return Weapon;
	}

	/**
	 * Sets weapon pointer
	 */
	void SetWeapon(AWeapon* NewWeapon)
	{
		// Warning on update
		if (Weapon != nullptr && NewWeapon != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon pointer already exists"));
		}

		Weapon = NewWeapon;
	}

protected:
	UPROPERTY()
	AWeapon* Weapon;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLESHOOTER_API UWeaponsInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * Weapon equip delegate
	 */
	FOnEquipCompleted OnEquipCompletedEvent;

	/**
	 * Starting equip delegate. Fires when async equip starts
	 */
	UPROPERTY(BlueprintAssignable, Category=Events)
	FOnEquipBegin OnEquipBegin;
	
	// Sets default values for this component's properties
	UWeaponsInventoryComponent();

	/**
	 * Gets currently equipped weapon
	 */
	UFUNCTION(BlueprintPure)
	AWeapon* GetEquippedWeapon() const;

	/**
	 * Pick ups a weapon
	 *
	 * @return bool - Pickup result
	 */
	UFUNCTION(BlueprintCallable) 
	bool Pickup(TSubclassOf<AWeapon> WeaponClass);

	/**
	 * PickUps an ammo for weapon
	 */
	UFUNCTION(BlueprintCallable)
	bool PickupAmmo(TSubclassOf<AWeapon> WeaponClass, int32 Amount);

	/**
	 * Simple equip event
	 */
	UFUNCTION(BlueprintCallable)
	void Equip(TSubclassOf<AWeapon> WeaponClass);

	/**
	 * Equip Prev Weapon in weapon list
	 */
	UFUNCTION(BlueprintCallable)
	bool EquipPreviousWeapon();

	/**
	 * Equip Next Weapon in weapon list
	 */
	UFUNCTION(BlueprintCallable)
	bool EquipNextWeapon();

	/**
	* Change weapon directly, i.e. here weapon will swapped
	*/
	UFUNCTION(BlueprintCallable)
	bool EquipSync(TSubclassOf<AWeapon> WeaponClass);

protected:
	/**
	 * Async weapon equipment
	 */
	UPROPERTY(BlueprintReadonly, EditDefaultsOnly)
	bool bAsyncEquip = true;
	
	virtual void BeginDestroy() override;
	
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(AllowPrivateAccess=true, DisplayName="Inventory"), Category=Config)
	TArray<FWeaponContainer> WeaponsContainers;

	/**
	 * Pointer for current equipped weapon
	 */
	UPROPERTY()
	AWeapon* CurrentEquippedWeapon;

	/**
	* Finds weapon in inventory
	*/
	FWeaponContainer* FindWeapon(TSubclassOf<AWeapon> WeaponClass);

	/**
	 * Find current weapon index in WeaponsSetup
	 */
	int32 FindCurrentWeaponSetupIndex();
};
