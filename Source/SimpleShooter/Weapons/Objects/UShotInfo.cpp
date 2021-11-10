// Fill out your copyright notice in the Description page of Project Settings.


#include "UShotInfo.h"

UUShotInfo::UUShotInfo()
{
	ShotGuid = FGuid::NewGuid();
}

void UUShotInfo::Init(AController* InstigatorController, TSubclassOf<AWeapon> Clazz)
{
	if (!ensure(bWasAlreadyInitialized == false))
	{
		return;
	}
	
	bWasAlreadyInitialized = true;
	Controller = InstigatorController;
	WeaponClass = Clazz;
}

void UUShotInfo::OnReceivesHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	OnReceivesHit(Hit);
}

void UUShotInfo::OnReceivesHit(const FHitResult& Hit) const
{
	const FShotHitInfo ShotInfo = CreateHitInfo(Hit);
	// ReSharper disable once CppExpressionWithoutSideEffects
	OnShotReceivesHitDelegate.ExecuteIfBound(&ShotInfo);
}

AController* UUShotInfo::GetInstigatorController() const
{
	return Controller;
}

FGuid UUShotInfo::GetGuid() const
{
	return ShotGuid;
}

FShotHitInfo UUShotInfo::CreateHitInfo(const FHitResult& Hit) const
{
	return FShotHitInfo(GetGuid(), Hit, WeaponClass, Controller);
}

UUShotInfo::~UUShotInfo()
{
	OnShotReceivesHitDelegate.Unbind();
}
