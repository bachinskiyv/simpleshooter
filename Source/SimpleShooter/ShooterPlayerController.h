// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModes/FreeForAllGameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Interfaces/ISimpleShooterController.h"

#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController, public IISimpleShooterController
{
	GENERATED_BODY()
	
public:
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;

	virtual void PostSeamlessTravel() override;

	virtual void GetSeamlessTravelActorList(bool bToEntry, TArray<AActor*>& ActorList) override;

	virtual TSubclassOf<AShooterCharacter> GetCharacterClass() const override;
	
	/**
	 * Postseamless travel for BP
	 */
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="PostSeamlessTravel"))
	void K2_PostSeamlessTravel();

	/**
	 * @TODO: Kostyl! Move messaging module to c++
	 */
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="TranslateMessage"))
	void K2_TranslateMessage(const FString& Message) const;

	/**
	 * Translate message
	 */
	virtual void TranslateMessage(const FString& Message) const override;

	/**
	 *	Gets player state
	 */
	virtual AShooterPlayerState* GetShooterPlayerState() const override;

	/**
	 * Gets shooter character
	 */
	virtual AShooterCharacter* GetShooterCharacter() const override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AShooterCharacter> PawnClass;
	
	virtual void BeginPlay() override;

	void OnMessageReceive(const FString& Message, ESimpleMessageType Type);
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinWidget;

	UPROPERTY(EditAnywhere)
	float RestartDelay = 5.f;

	FTimerHandle RestartTimer;

	bool bCanShowWinWidget = true;
};
