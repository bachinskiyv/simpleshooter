
#pragma once

#include "Engine/EngineTypes.h"
#include "SimpleShooter/Weapons/Objects/UShotInfo.h"

#include "DamageTypes.generated.h"

USTRUCT()
struct FPointDamageWithGuidEvent : public FPointDamageEvent
{
	GENERATED_BODY()

	FPointDamageWithGuidEvent()
		: FPointDamageEvent()
	{}
	
	FPointDamageWithGuidEvent(float InDamage, struct FHitResult const& InHitInfo, FVector const& InShotDirection, TSubclassOf<class UDamageType> InDamageTypeClass)
		: FPointDamageEvent(InDamage, InHitInfo, InShotDirection, InDamageTypeClass)
	{}
	
	FShotHitInfo ShotHitInfo;
};
