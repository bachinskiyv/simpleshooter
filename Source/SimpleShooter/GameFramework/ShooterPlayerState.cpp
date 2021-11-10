// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerState.h"

#include "SimpleShooter/ShooterPlayerController.h"

AShooterPlayerState::AShooterPlayerState()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AShooterPlayerState::CheckAndRerunExcellentChallenge()
{
	if (GetWorldTimerManager().IsTimerActive(ExcellentKillTimerHandle))
	{
		// Raise excellent
	}
	
	GetWorldTimerManager().SetTimer(
		ExcellentKillTimerHandle,
		ExcellentKillTimeout,
		false,
		0.f
	);
}

void AShooterPlayerState::CollectVictimHit(const FShotHitInfo ShotHitInfo)
{
	
}

// @TODO: MOVE FROM PLAYER STATE
void AShooterPlayerState::CollectVictimDeath(const IISimpleShooterController* KillerController, const IISimpleShooterController* VictimController, const FShotHitInfo DeadHitInfo, FDamageEvent DamageEvent)
{
	if (KillerController != nullptr && VictimController != nullptr)
	{
		CheckAndRerunExcellentChallenge();

		if (KillerController == VictimController)
		{
			const ECharacterGender Gender = KillerController->GetShooterCharacter()->GetGender();

			FString SuicideGender;
			switch (Gender)
			{
				case ECharacterGender::Male:
					SuicideGender = TEXT("himself");
					break;
				case ECharacterGender::Female:
					SuicideGender = TEXT("herself");
					break;
				default:
					SuicideGender = TEXT("itself");
					break;
			}

			KillerController->TranslateMessage(
				FString::Printf(
					TEXT("%s killed %s"),
					*KillerController->GetShooterPlayerState()->GetPlayerName(),
					*SuicideGender
				)
			);
		}
		else
		{
			FString KillString;
			DetermineKillText(
				*KillerController->GetShooterPlayerState()->GetPlayerName(),
				*VictimController->GetShooterPlayerState()->GetPlayerName(),
				DeadHitInfo.WeaponClass,
				DamageEvent.DamageTypeClass,
				KillString
			);

			KillerController->TranslateMessage(KillString);
		}
	}
}
