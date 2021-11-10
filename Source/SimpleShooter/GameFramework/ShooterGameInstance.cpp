// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

void UShooterGameInstance::Init()
{
	const auto Subsystem = IOnlineSubsystem::Get();
	if (!ensure(Subsystem != nullptr))
	{
		return;
	}

	SessionInterface = Subsystem->GetSessionInterface();

	if (!ensure(SessionInterface.IsValid()))
	{
		return;
	}

	// Add Delegates on Events
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UShooterGameInstance::OnCreateSessionComplete);
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UShooterGameInstance::OnDestroySessionComplete);
}

void UShooterGameInstance::Host(FString ServerName)
{
	LastDesiredServerName = ServerName;
	if (ensure(SessionInterface.IsValid()))
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UShooterGameInstance::CreateSession()
{
	if (!ensure(SessionInterface.IsValid()))
	{
		return;
	}

	FOnlineSessionSettings SessionSettings;
	
	SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSettings.NumPublicConnections = 5;
	// @TODO needed? What is it?
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, LastDesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);	
}

void UShooterGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not create session"));
		
		return;
	}

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Sandbox?listen");
}

void UShooterGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}
