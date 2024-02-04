// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UGroomComponent;
class AFItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class UEBASICS_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlashCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	* Callbacks for input
	*/

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* SlashContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	void Move(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookingAction;

	void Looking(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	virtual void Jump() override;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EKeyAction;

	void EKey();

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	void Attack();

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction;

	void Dodge();

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* HairMesh;

	//
	//Play montage functions
	//

	void PlayAttackMontage();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	bool CanAttack();

	void PlayEquipMontage(const FName& SectionName);

	bool CanDisarm();

	bool CanArm();

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquipedWeapon;

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponColisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	//UFUNCTION(BlueprintCallable)
	//void SetWeaponColisionDisabled(ECollisionEnabled::Type CollisionEnabled);

private:

	ECharacterStates CharacterState = ECharacterStates::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AFItem* OverlappingItem;

	//
	//Animation montages
	//
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

public:

	FORCEINLINE void SerOverlappingItem(AFItem* Item) { OverlappingItem = Item; }

	FORCEINLINE ECharacterStates GetCharacterState() const { return CharacterState; }

};
