// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorComponent.h"

// Sets default values for this component's properties
UArmorComponent::UArmorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

float UArmorComponent::TakeDamage(float DamageAmount)
{
	// No Damage to armor
	if (Armor == 0 || AbsorptionRatio <= 0)
	{
		return DamageAmount;
	}

	if (DamageAmount == 0)
	{
		return DamageAmount;
	}

	// Calculate real damage by ratio
	float ArmorDamage = DamageAmount * AbsorptionRatio;
	float NewDamageAmount = DamageAmount - ArmorDamage;

	if (ArmorDamage > Armor)
	{
		/**
		 * Example: (100Hp/20Ap 60Dmg/0.4Absorb)
		 *
		 * ArmorDamage 60 * 0.4 = 24
		 * NewDamageAmount = 60 - 24 = 36
		 *
		 * Armor Damage much more than Armor
		 * 24 - 20 = 4 AP
		 * 4 / 0.4 = 10 HP
		 *
		 * 36 + 10 = 46 instead of 60, in 0.4 absorbtion effect
		 */
		NewDamageAmount += (ArmorDamage - Armor) / AbsorptionRatio;
		
		Armor = 0;
	} else
	{
		Armor -= ArmorDamage;
	}

	return NewDamageAmount;
}

void UArmorComponent::SetCurrentAbsorptionRatio(float Ratio)
{
	AbsorptionRatio = FMath::Clamp(Ratio, 0.f, 1.f);
}

void UArmorComponent::SetArmorValue(float NewValue)
{
	Armor = FMath::Clamp(NewValue, 0.f, MaxArmor);
}

float UArmorComponent::GetArmorValue() const
{
	return Armor;
}
