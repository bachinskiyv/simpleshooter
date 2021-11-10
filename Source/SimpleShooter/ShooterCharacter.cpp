// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Weapons/Weapon.h"
#include "SimpleShooterGameModeBase.h"
#include "Components/ArmorComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "Components/WeaponsInventoryComponent.h"
#include "GameFramework/ShooterPlayerState.h"
#include "Weapons/Actors/EnrichedProjectileBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	ArmorComponent = CreateDefaultSubobject<UArmorComponent>(TEXT("Armor"));
	WeaponsInventoryComponent = CreateDefaultSubobject<UWeaponsInventoryComponent>(TEXT("Weapons"));
	
	WeaponsInventoryComponent->OnEquipCompletedEvent.BindUObject(this, &AShooterCharacter::OnWeaponEquip);

	OnDestroyed.AddDynamic(this, &AShooterCharacter::OnActorDestroyed);
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Hide Weapon mesh
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
}

void AShooterCharacter::OnWeaponEquip(AWeapon* Weapon)
{	
	Weapon->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		TEXT("WeaponSocket")
	);

	Weapon->SetOwner(this);
}

bool AShooterCharacter::IsDead() const
{
	return HealthComponent->IsDead();
}

int32 AShooterCharacter::GetHealth() const
{
	return FMath::RoundToInt(HealthComponent->GetHealth());
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookAround"), this, &AShooterCharacter::LookAround);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AShooterCharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AShooterCharacter::StartAttack);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AShooterCharacter::StopAttack);
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	AEnrichedProjectileBase* EnrichedProjectile = Cast<AEnrichedProjectileBase>(DamageCauser);
	if (EnrichedProjectile != nullptr)
	{
		FHitResult HitResult;
		FVector OutImpulseDir;
		DamageEvent.GetBestHitInfo(this, EventInstigator, HitResult, OutImpulseDir);
		FShotHitInfo ShotHitInfo = EnrichedProjectile->GetShotInfo()->CreateHitInfo(HitResult);
		TakeDamage(ShotHitInfo, DamageAmount, DamageEvent, DamageCauser);
	}
	else
	{
		Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);	
	}
	
	return DamageAmount;
}

float AShooterCharacter::TakeDamage(FShotHitInfo HitInfo, float DamageAmount, FDamageEvent const& DamageEvent, AActor* DamageCauser)
{
	// Already dead
	if (IsDead())
	{
		return 0.f;	
	}

	AController* EventInstigator = HitInfo.Controller;
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = ArmorComponent->TakeDamage(DamageToApply);
	DamageToApply = FMath::Min(HealthComponent->GetHealth(), DamageToApply);
	HealthComponent->UpdateHealth(HealthComponent->GetHealth() - DamageToApply);

	AShooterPlayerState* LocalPlayerState = nullptr;
	if (EventInstigator != nullptr)
	{
		LocalPlayerState = EventInstigator->GetPlayerState<AShooterPlayerState>();
	}

	// Dead now
	if (IsDead())
	{
		IISimpleShooterController* KillerController = Cast<IISimpleShooterController>(EventInstigator);
		IISimpleShooterController* VictimController = Cast<IISimpleShooterController>(GetController());
		if (LocalPlayerState != nullptr)
		{
			LocalPlayerState->CollectVictimDeath(KillerController, VictimController, HitInfo, DamageEvent);
		}

		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{	
			GameMode->PawnKilled(this, EventInstigator);
		}
		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetCanBeDamaged(false);
	}
	else
	{
		if (LocalPlayerState != nullptr)
		{
			LocalPlayerState->CollectVictimHit(HitInfo);
		}	
	}

	return DamageToApply;
}

void AShooterCharacter::UnPossessed()
{
	StopAttack();
	
	Super::UnPossessed();
}

void AShooterCharacter::OnActorDestroyed(AActor* DestroyedActor)
{
	AWeapon* Weapon = WeaponsInventoryComponent->GetEquippedWeapon();
	if (Weapon != nullptr)
	{
		// Destroying actor is not immediate, so weapon remains in hands
		Weapon->SetActorHiddenInGame(true);
	}
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AShooterCharacter::LookAround(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AShooterCharacter::Jump()
{
	Super::Jump();
}

void AShooterCharacter::StartAttack()
{
	AWeapon* Weapon = WeaponsInventoryComponent->GetEquippedWeapon();
	if (Weapon)
	{
		Weapon->StartShooting();
	}
}

void AShooterCharacter::StopAttack()
{
	AWeapon* Weapon = WeaponsInventoryComponent->GetEquippedWeapon();
	if (Weapon)
	{
		Weapon->StopShooting();
	}
}

AController* AShooterCharacter::GetController() const
{
	return Controller;
}

ECharacterGender AShooterCharacter::GetGender() const
{
	return Gender;
}

bool AShooterCharacter::CanShoot()
{
	return !HealthComponent->IsDead();
}



