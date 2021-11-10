// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/ShooterPlayerState.h"
#include "GameFramework/GameModes/FreeForAllGameMode.h"
#include "GameFramework/HUDs/ASkirmishHUD.h"

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (bIsWinner)
	{
		// @TODO: вообще-то костыль
		if (bCanShowWinWidget)
		{
			bCanShowWinWidget = false;
			UUserWidget* WinScreen = CreateWidget(this, WinWidget);
			if (WinScreen != nullptr)
			{
				WinScreen->AddToViewport();
			}
		}
	}
	else
	{
		UUserWidget* LoseScreen = CreateWidget(this, LoseWidget);
		if (LoseScreen != nullptr)
		{
			LoseScreen->AddToViewport();
		}
	}
	
	GetWorldTimerManager().SetTimer(
		RestartTimer,
		this,
		&APlayerController::RestartLevel,
		RestartDelay
	);
}

void AShooterPlayerController::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	// TODO: Duplicate
	auto GameMode = GetWorld()->GetAuthGameMode<AFreeForAllGameMode>();
	if (GameMode != nullptr)
	{
		GameMode->SimpleMessageRelayDelegate.AddUObject(this, &AShooterPlayerController::OnMessageReceive);
	}

	K2_PostSeamlessTravel();
}

void AShooterPlayerController::GetSeamlessTravelActorList(bool bToEntry, TArray<AActor*>& ActorList)
{
	if (MyHUD != nullptr)
	{
		MyHUD->Destroy();
		MyHUD = nullptr;
	}

	Super::GetSeamlessTravelActorList(bToEntry, ActorList);

	AShooterPlayerState* TransferPlayerState = GetPlayerState<AShooterPlayerState>();
	if (TransferPlayerState != nullptr)
	{
		ActorList.Add(TransferPlayerState);
	}
}

TSubclassOf<AShooterCharacter> AShooterPlayerController::GetCharacterClass() const
{
	return PawnClass;
}

void AShooterPlayerController::TranslateMessage(const FString& Message) const
{
	const auto GameMode = GetWorld()->GetAuthGameMode<AFreeForAllGameMode>();
	if (GameMode != nullptr)
	{
		GameMode->SimpleMessageRelayDelegate.Broadcast(Message, ESimpleMessageType::System);
	}
}

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto GameMode = GetWorld()->GetAuthGameMode<AFreeForAllGameMode>();
	if (GameMode != nullptr)
	{
		GameMode->SimpleMessageRelayDelegate.AddUObject(this, &AShooterPlayerController::OnMessageReceive);
	}
}

void AShooterPlayerController::OnMessageReceive(const FString& Message, ESimpleMessageType Type)
{
	AASkirmishHUD* SkirmishHUD = Cast<AASkirmishHUD>(GetHUD());
	if (SkirmishHUD != nullptr)
	{
		SkirmishHUD->DrawMessage(Message);
	}
}

AShooterPlayerState* AShooterPlayerController::GetShooterPlayerState() const
{
	return Cast<AShooterPlayerState>(PlayerState);
}

AShooterCharacter* AShooterPlayerController::GetShooterCharacter() const
{
	return GetPawn<AShooterCharacter>();
}

