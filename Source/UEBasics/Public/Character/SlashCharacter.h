// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Interface/PickupInterface.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UGroomComponent;
class AFItem;
class UAnimMontage;
class ASoul;
class ATreasure;

UCLASS()
class UEBASICS_API ASlashCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:

	ASlashCharacter();
	virtual void Tick(float DeltaTime)override;

	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/* <IHitInterface> */

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	virtual void SetOverlappingItem(AFItem* Item)override;
	virtual void AddSouls(ASoul* Soul)override;
	virtual void AddGold(ATreasure* Gold)override;

protected:

	virtual void BeginPlay() override;

	/*
	* Callbacks for inputs
	*/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& Value);
	void Looking(const FInputActionValue& Value);
	virtual void Jump() override;
	void EKey();
	virtual void Attack()override;
	void Dodge();

	bool HasStamina();

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* SlashContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookingAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EKeyAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeAction;

	//
	//Combat
	//
	bool CanArm();
	bool CanDisarm();
	void Arm();
	void Disarm();
	virtual bool CanAttack()override;
	void EquipWeapon(AWeapon* Weapon);
	virtual void AttackEnd()override;
	void PlayEquipMontage(const FName& SectionName);
	virtual void Die_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void DodgeEnd();
	
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void EndReact();

private:

	void SetHUDHealth();

	// Character components //
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* HairMesh;

	UPROPERTY(VisibleInstanceOnly)
	AFItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere)
	ECharacterStates CharacterState = ECharacterStates::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;
	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DodgeMontage;

	UPROPERTY()
	class UPlayerOverlay* PlayerOverlay;

public:

	//FORCEINLINE void SerOverlappingItem(AFItem* Item) { OverlappingItem = Item; }

	FORCEINLINE ECharacterStates GetCharacterState() const { return CharacterState; }

	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};
