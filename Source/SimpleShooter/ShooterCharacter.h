// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Character/Damage/DamageTypes.h"
#include "GameFramework/Character.h"
#include "Weapons/Interfaces/IPawnHoldsWeapon.h"

#include "ShooterCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterGender : uint8
{
	Male UMETA(DisplayName = "Male"),
	Female UMETA(DisplayName = "Female"),
	It UMETA(DisplayName = "It")
};

class AWeapon;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter, public IIPawnHoldsWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	/**
	 * Setup Gender
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECharacterGender Gender = ECharacterGender::Male;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnWeaponEquip(AWeapon* Weapon);

public:
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	int32 GetHealth() const;

	void StartAttack();
	void StopAttack();

	// @TODO: NO-SHADOWING
	// region IIPawnHoldsWeapon
	AController* GetController() const;

	ECharacterGender GetGender() const;

	virtual bool CanShoot() override;
	// endregion IIPawnHoldsWeapon

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	float TakeDamage(FShotHitInfo HitInfo, float DamageAmount, FDamageEvent const& DamageEvent, AActor* DamageCauser);
	
	virtual void UnPossessed() override;

	UFUNCTION()
	void OnActorDestroyed(AActor* DestroyedActor);

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookAround(float AxisValue);

	virtual void Jump() override;

	UPROPERTY(EditAnywhere)
	float RotationRate = 10;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category="Components", VisibleDefaultsOnly)
	class UHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category="Components", VisibleDefaultsOnly)
	class UArmorComponent* ArmorComponent; 

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category="Components", VisibleDefaultsOnly)
	class UWeaponsInventoryComponent* WeaponsInventoryComponent; 
};
