// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;
class UAnimMontage;

UCLASS()
class UEBASICS_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	bool isAlive();
	void PlayDeathMontage();

	virtual bool CanAttack();

	virtual void Attack();

	virtual void HandleDamage(float DamageAmount);

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	UFUNCTION(BlueprintNativeEvent)
	void Die();

	void DisableMeshCollision();

	void PlayAttackMontage();

	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	void PlayHitReactMontage(const FName& SectionName);

	void DirectionHitReact(const FVector& ImpactPoint);

	void PlayHitSound(const FVector& ImpactPoint);

	void PlayHitParticles(const FVector& ImpactPoint);

	void DisableCapsule();

	void StopAttackMontage();

	UFUNCTION(BlueprintCallable)
	FVector GetWarpTargetTranslation();

	UFUNCTION(BlueprintCallable)
	FVector GetWarpTargetRotation();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void SetWeaponColisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquipedWeapon;

	UPROPERTY(BlueprintReadOnly, Category = Combat)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = Combat)
	double WarpTargetDistance = 75.f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int SelectedDeathAnimaton;

private:

	//
	//Animation montages
	//
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Particles)
	UParticleSystem* HitParticles;

public:

	FORCEINLINE int GetSelectedDeathAnimaton() const { return SelectedDeathAnimaton; }
};
