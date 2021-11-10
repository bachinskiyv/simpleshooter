// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"


UCLASS(Abstract)
class SIMPLESHOOTER_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * Can you grab it right now?
	 */
	UFUNCTION(BlueprintPure)
	bool CanPickUp();

	/**
	 * Grab a pickup
	 */
	UFUNCTION(BlueprintCallable)
	void Pickup();
	
protected:
	/**
	 * Respawn pickup time
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float RespawnTime = 10.f;

private:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly)
	class UBoxComponent* Collision;

	bool bCanPickUp = true;

	FTimerHandle ResetTimerHandle;

	void ResetPickup();
};
