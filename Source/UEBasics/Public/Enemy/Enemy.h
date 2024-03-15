// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Character/CharacterTypes.h"
#include "UObject/ObjectPtr.h"
#include "UObject/WeakObjectPtr.h"
#include "Enemy.generated.h"


class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class UEBASICS_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()


public:

	AEnemy();

	/* <AActor> */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	virtual void Destroyed()override;
	/* <AActor> */


	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	/* <IHitInterface> */

protected:

	/* <AActor> */
	virtual void BeginPlay() override;
	/* <AActor> */


	/* <ABaseCharacter> */
	virtual bool CanAttack()override;
	virtual void Attack()override;
	virtual void HandleDamage(float DamageAmount)override;
	virtual void AttackEnd()override;
	virtual void Die_Implementation()override;
	void SpawnSoul();
	/* <ABaseCharacter> */

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EEnenyStates EnemyState = EEnenyStates::EAS_Patrolling;

	UPROPERTY(EditAnywhere, Category = Combat)
	double AcceptanceRadius = 50.f;


private:



	// AI behavior //
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	AActor* ChoosePatrolTarget();
	void PatrolTimerFinished();
	void ShowHealthBar(bool toShow);
	void StartPatrolling();
	void LooseInterest();
	bool IsOutsideCombatRadius();
	void ChaseTarget();
	bool IsInAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool isDead();
	bool isInCombat();
	void SpawnDefaultWeapon();

	void StartAttackTimer();
	void ClearAttackTimer();
	void MoveTo(AActor* Target);
	bool InRange(AActor* Target, double Radius);
	
	UFUNCTION()
	void DetectTarget(APawn* PawnSeen); //Callback for OnPawnSeen in UPawnSensingComponent
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = Combat)
	double CombatRadius = 500;

	UPROPERTY(EditAnywhere, Category = Combat)
	double AttackRadius = 150;


	UPROPERTY()
	class AAIController* EnemyController;

	//Current patrol target
	UPROPERTY(EditAnywhere, Category = Particles)
	AActor* PatrolTarget;

	UPROPERTY(EditAnywhere, Category = Particles)
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = Combat)
	double PatrolRadius = 200;


	FTimerHandle PatrolTimer;
	UPROPERTY(EditAnywhere, Category = "AI navigation")
	float PatrolWaitMax = 10.f;
	UPROPERTY(EditAnywhere, Category = "AI navigation")
	float PatrolWaitMin = 5.f;

	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpam = 5.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float WalkSpeed = 125.f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class ASoul> SoulClass;


	public:

	//UFUNCTION(BlueprintReadOnly, VisibleAnywhere)
	//FORCEINLINE EEnenyStates GetCurrentEnemyState() const { return EnemyState; }
};
