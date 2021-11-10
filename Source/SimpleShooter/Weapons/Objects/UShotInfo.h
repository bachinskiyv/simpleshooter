// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SimpleShooter/Weapons/Weapon.h"
#include "Misc/Guid.h"
#include "UShotInfo.generated.h"

// @todo: no need bp type
USTRUCT(BlueprintType)
struct FShotHitInfo
{
	GENERATED_BODY()

	FShotHitInfo(): Controller(nullptr) {}

	FShotHitInfo(FGuid ShotGuid, const FHitResult& HitResult, TSubclassOf<AWeapon> WeaponClass, AController* InstigatorController)
		: ShotGuid(ShotGuid), HitResult(HitResult), WeaponClass(WeaponClass), Controller(InstigatorController)
	{}

	FGuid ShotGuid;
	FHitResult HitResult;
	TSubclassOf<AWeapon> WeaponClass;
	
	UPROPERTY()
	AController* Controller;
};

DECLARE_DELEGATE_OneParam(FOnShotReceivesHitSignature, const FShotHitInfo*);

/**
 * 
 */
// @TODO: no need
UCLASS(BlueprintType)
class SIMPLESHOOTER_API UUShotInfo : public UObject
{
	GENERATED_BODY()
	
public:
	FOnShotReceivesHitSignature OnShotReceivesHitDelegate;
	
	UUShotInfo();
	
	void Init(AController* InstigatorController, TSubclassOf<AWeapon> Clazz);

	UFUNCTION()
	void OnReceivesHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse,
											const FHitResult& Hit);

	void OnReceivesHit(const FHitResult& Hit) const;
	
	AController* GetInstigatorController() const;

	FGuid GetGuid() const;

	FShotHitInfo CreateHitInfo(const FHitResult& Hit) const;

	virtual ~UUShotInfo() override;

private:
	bool bWasAlreadyInitialized = false;

	FGuid ShotGuid;

	UPROPERTY()
	AController* Controller;

	TSubclassOf<AWeapon> WeaponClass;
};
