// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponsInventoryComponent.h"

// Sets default values for this component's properties
UWeaponsInventoryComponent::UWeaponsInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

AWeapon* UWeaponsInventoryComponent::GetEquippedWeapon() const
{
	return CurrentEquippedWeapon;
}

// Called when the game starts
void UWeaponsInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Equip weapons at start
	for (const auto& WeaponData : WeaponsContainers)
	{
		if (WeaponData.bPickedUpAtStart)
		{
			if (Pickup(WeaponData.WeaponClass))
			{
				if (WeaponData.bIsEquippedAtStart)
				{
					EquipSync(WeaponData.WeaponClass);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Cannot pickup weapon with class: %s"), *WeaponData.WeaponClass->GetName());
			}
		}
	}
}

FWeaponContainer* UWeaponsInventoryComponent::FindWeapon(TSubclassOf<AWeapon> WeaponClass)
{
	FWeaponContainer* FoundedContainer = WeaponsContainers.FindByPredicate([WeaponClass](const FWeaponContainer& WeaponContainer)
	{
		return WeaponContainer.WeaponClass == WeaponClass;
	});

	return FoundedContainer;
}

int32 UWeaponsInventoryComponent::FindCurrentWeaponSetupIndex()
{
	if (CurrentEquippedWeapon != nullptr && WeaponsContainers.Num() > 1)
	{
		const int32 Index = WeaponsContainers.IndexOfByPredicate([this](const FWeaponContainer& WeaponContainer)
		{
			return CurrentEquippedWeapon == WeaponContainer.GetWeapon();
		});

		if (ensure(Index != INDEX_NONE))
		{
			return Index;
		}
	}

	return INDEX_NONE;
}

bool UWeaponsInventoryComponent::Pickup(TSubclassOf<AWeapon> WeaponClass)
{
	// We must have an owner
	if (!ensure(GetOwner() != nullptr))
	{
		return false;
	}

	// Cannot pickup non-existed weapon
	FWeaponContainer* WeaponContainerPtr = FindWeapon(WeaponClass);
	if (WeaponContainerPtr == nullptr)
	{
		return false;
	}
	
	AWeapon* Weapon = WeaponContainerPtr->GetWeapon();
	if (Weapon == nullptr)
	{
		AActor* Owner = GetOwner();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = Owner;

		AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, SpawnParameters);
		if (!ensure(NewWeapon != nullptr))
		{
			return false;
		}
		
		WeaponContainerPtr->SetWeapon(NewWeapon);
	}
	else
	{
		return Weapon->PickAmmo(Weapon->GetInitialAmmoAmount());
	}
	
	return true;
}

bool UWeaponsInventoryComponent::PickupAmmo(TSubclassOf<AWeapon> WeaponClass, int32 Amount)
{
	FWeaponContainer* WeaponContainerPtr = FindWeapon(WeaponClass);
	if (WeaponContainerPtr == nullptr)
	{
		return false;
	}
	
	AWeapon* Weapon = WeaponContainerPtr->GetWeapon();
	if (Weapon != nullptr)
	{
		return Weapon->PickAmmo(Amount);
	}

	return false;
}

/**
 *  @TODO: return bool
 */
void UWeaponsInventoryComponent::Equip(TSubclassOf<AWeapon> WeaponClass)
{
	// Do not switch weapon to itself
	if (CurrentEquippedWeapon != nullptr && CurrentEquippedWeapon->IsA(WeaponClass))
	{
		return;
	}
	
	if (bAsyncEquip)
	{
		OnEquipBegin.Broadcast(WeaponClass);	
	}
	else
	{
		EquipSync(WeaponClass);
	}
}

bool UWeaponsInventoryComponent::EquipSync(TSubclassOf<AWeapon> WeaponClass)
{
	// Cannot equip non-existed weapon
	FWeaponContainer* WeaponContainerPtr = FindWeapon(WeaponClass);
	if (WeaponContainerPtr == nullptr)
	{
		return false;
	}
	
	AWeapon* Weapon = WeaponContainerPtr->GetWeapon();
	if (Weapon != nullptr)
	{
		if (CurrentEquippedWeapon != nullptr)
		{
			CurrentEquippedWeapon->SetActorHiddenInGame(true);
		}

		Weapon->SetActorHiddenInGame(false);

		/**
		* @TODO: WEAK PTR?
		*/
		CurrentEquippedWeapon = Weapon;
		OnEquipCompletedEvent.Execute(Weapon);

		return true;
	}

	return false;
}


/**
 * @TODO: Add a parameter - CheckAmmo
 */
bool UWeaponsInventoryComponent::EquipPreviousWeapon()
{
	const int32 CurrentWeaponIndex = FindCurrentWeaponSetupIndex();
	// We found an index and we can rewind 
	if (CurrentWeaponIndex != INDEX_NONE)
	{
		// Walk through zero index
		const int32 RelativeIndex = CurrentWeaponIndex > 0 ? CurrentWeaponIndex : WeaponsContainers.Num();
		
		// Starts from previous weapon
		for (int32 Index = RelativeIndex - 1; Index >= 0; --Index)
		{
			const AWeapon* Weapon = WeaponsContainers[Index].GetWeapon();
			if (Weapon != nullptr)
			{
				Equip(WeaponsContainers[Index].WeaponClass);

				return true;
			}
		}
	}

	return false;
}

bool UWeaponsInventoryComponent::EquipNextWeapon()
{
	const int32 LastIndex = WeaponsContainers.Num() - 1;
	const int32 CurrentWeaponIndex = FindCurrentWeaponSetupIndex();
	// We found an index and we can forward
	if (CurrentWeaponIndex != INDEX_NONE)
	{
		// Walk through last index
		const int32 RelativeIndex = CurrentWeaponIndex < LastIndex ? CurrentWeaponIndex : -1;
		
		// Starts from next weapon
		for (int32 Index = RelativeIndex + 1; Index <= LastIndex; ++Index)
		{
			const AWeapon* Weapon = WeaponsContainers[Index].GetWeapon();
			if (Weapon != nullptr)
			{
				Equip(WeaponsContainers[Index].WeaponClass);

				return true;
			}
		}
	}
	
	return false;
}

/**
 * @TODO: WEAK PTR?
 */
void UWeaponsInventoryComponent::BeginDestroy()
{
	Super::BeginDestroy();

	for (FWeaponContainer& WeaponContainer : WeaponsContainers)
	{
		AWeapon* Weapon = WeaponContainer.GetWeapon();

		if (Weapon != nullptr)
		{
			Weapon->Destroy();
		}
		
		WeaponContainer.SetWeapon(nullptr);
	}
}

