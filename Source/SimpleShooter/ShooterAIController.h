// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterCharacter.h"
#include "GameFramework/Interfaces/ISimpleShooterController.h"

#include "ShooterAIController.generated.h"


/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterAIController : public AAIController, public IISimpleShooterController
{
	GENERATED_BODY()
public:
	AShooterAIController();
	
	bool IsDead() const;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void PostInitializeComponents() override;

	virtual TSubclassOf<AShooterCharacter> GetCharacterClass() const override;

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
	 * Gets player state
	 */
	virtual AShooterPlayerState* GetShooterPlayerState() const override;

	/**
	 * Gets character
	 */
	virtual AShooterCharacter* GetShooterCharacter() const override;
	
protected:
	/**
	 * Custom player name
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly);
	FString PlayerName;

	/**
	 * Player pawn model
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly);
	TSubclassOf<AShooterCharacter> PawnClass;

private:
	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* AIBehavior;
};
