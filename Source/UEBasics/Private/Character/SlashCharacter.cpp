// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SlashCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Animation/AnimMontage.h"
#include "Items/Weapons/Weapon.h"
#include "Components/StaticMeshComponent.h"
#include "HUD/PlayerHUD.h"
#include "HUD/PlayerOverlay.h"
#include "Component/AttributeComponent.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.0f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	HairMesh->SetupAttachment(GetMesh(), TEXT("headSocket"));

}

void ASlashCharacter::Tick(float DeltaTime)
{
	if (Attributes && PlayerOverlay)
	{
		Attributes->RegenerateStamina(DeltaTime);
		PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponColisionEnabled(ECollisionEnabled::NoCollision);

	if (Attributes && Attributes->GetHealthPercent() >0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
	
}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageCauser)
	{
		HandleDamage(DamageAmount && Attributes);
		SetHUDHealth();
	}
	
	return DamageAmount;
}

void ASlashCharacter::SetOverlappingItem(AFItem* Item)
{
	OverlappingItem = Item;
}

void ASlashCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && PlayerOverlay)
	{
		Attributes->AddSoul(Soul->GetSouls());
		PlayerOverlay->SetSouls(Attributes->GetSouls());
	}
}

void ASlashCharacter::AddGold(ATreasure* Gold)
{
	if (Attributes && PlayerOverlay)
	{
		Attributes->AddGold(Gold->GetGold());
		PlayerOverlay->SetGold(Attributes->GetGold());
	}
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashContext, 0);
		}
		APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
		if (PlayerHUD)
		{
			PlayerOverlay =  PlayerHUD->GetPlayerOverlay();
			if (PlayerOverlay && Attributes)
			{
				PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				PlayerOverlay->SetStaminaBarPercent(1.f);
				PlayerOverlay->SetGold(0);
				PlayerOverlay->SetSouls(0);
			}
		}
	}
	
	Tags.Add(FName("Player"));
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(LookingAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Looking);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EKey);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Dodge);
	}
}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator ControlRotaion = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotaion.Yaw, 0.f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ASlashCharacter::Looking(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);

}

void ASlashCharacter::Jump()
{
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		Super::Jump();
	}

}

void ASlashCharacter::EKey()
{
	AWeapon* OwerlappingWeapon = Cast<AWeapon>(OverlappingItem);

	if (OwerlappingWeapon)
	{
		if (EquipedWeapon)
		{
			EquipedWeapon->Destroy();
		}
		EquipWeapon(OwerlappingWeapon);
	}
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

void ASlashCharacter::Attack()
{
	Super::Attack();

	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
	
}

void ASlashCharacter::Dodge()
{
	if (ActionState != EActionState::EAS_Unoccupied || !HasStamina())return;

	PlayMontageSection(DodgeMontage, FName("Default"));
	ActionState = EActionState::EAS_Dodge;
	if (PlayerOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCost());
		PlayerOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}

}

bool ASlashCharacter::HasStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterStates::ECS_Unequipped &&
		EquipedWeapon;
}

bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterStates::ECS_Unequipped;
}

void ASlashCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterStates::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterStates::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

bool ASlashCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterStates::ECS_Unequipped;
}

void ASlashCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterStates::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquipedWeapon = Weapon;
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ASlashCharacter::Die_Implementation()
{
	Super::Die_Implementation();
	ActionState = EActionState::EAS_Dead;
}

void ASlashCharacter::DodgeEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::AttachWeaponToBack()
{
	if (EquipedWeapon)
	{
		EquipedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCharacter::AttachWeaponToHand()
{
	if (EquipedWeapon)
	{
		EquipedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::EndReact()
{
	ActionState = EActionState::EAS_Unoccupied;
}


void ASlashCharacter::SetHUDHealth()
{
	if (PlayerOverlay)
	{
		PlayerOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}




