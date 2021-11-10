// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArmorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLESHOOTER_API UArmorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UArmorComponent();

	/**
	 * Strikes the armor
	 *
	 * @param DamageAmount Taken Damage Amount
	 *
	 * @returns Remaining Damage amount, which armor did not absorb
	 */
	float TakeDamage(float DamageAmount);

protected:
	/**
	 * Absorption ratio for armor. Must be clamped to (0, 1)
	 */
	UPROPERTY(BlueprintReadOnly)
	float AbsorptionRatio;

	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetArmorValue() const;

	/**
	* Update Absorption ratio when we wear new armor
	*
	* @param Ratio 0..1
	*/
	UFUNCTION(BlueprintCallable)
	void SetCurrentAbsorptionRatio(float Ratio);

	UFUNCTION(BlueprintCallable)
	void SetArmorValue(float NewValue);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MaxArmor = 100;
private:
	float Armor = 0;
};
