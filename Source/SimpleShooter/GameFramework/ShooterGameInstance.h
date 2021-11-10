// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "ShooterGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;
	
	UFUNCTION(Exec)
	void Host(FString ServerName);

private:
	IOnlineSessionPtr SessionInterface;

	/**
	 * @todo: Must be synchronized with createSession, i think
	 */
	FString LastDesiredServerName;

	void CreateSession();
	
	void OnCreateSessionComplete(FName SessionName, bool Success);

	void OnDestroySessionComplete(FName SessionName, bool Success);
};
