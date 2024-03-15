// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Component/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Items/Weapons/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Soul.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);

}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isDead())
	{
		EnemyState = EEnenyStates::EAS_Dead;
		return;
	}

	if (EnemyState > EEnenyStates::EAS_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	if (IsInAttackRadius())
	{
		EnemyState = EEnenyStates::EAS_Attacking;
	}
	else
	{
		ChaseTarget();
	}

	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (EquipedWeapon)
	{
		EquipedWeapon->Destroy();
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	if (!isDead()) ShowHealthBar(true);

	SetWeaponColisionEnabled(ECollisionEnabled::NoCollision);
	GetWorldTimerManager().ClearTimer(PatrolTimer);
	ClearAttackTimer();

	
	StopAttackMontage();

	if (IsInAttackRadius() && isAlive())
	{
		StartAttackTimer();
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::DetectTarget);

	InitializeEnemy();
	Tags.Add(FName("Enemy"));

}

bool AEnemy::CanAttack()
{
	bool bCanAttack =
		IsInAttackRadius() &&
		!IsAttacking() &&
		!isInCombat() &&
		!isDead();

	return bCanAttack;
}

void AEnemy::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr)return;
	//EnemyState = EEnenyStates::EAS_Combat;
	EnemyState = EEnenyStates::EAS_Attacking;
	PlayAttackMontage();
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnenyStates::EAS_NoState;
	CheckCombatTarget();
}

void AEnemy::Die_Implementation()
{
	if (ActorHasTag(FName("Dead")))
	{
		EnemyState = EEnenyStates::EAS_Dead;

	}

	Super::Die_Implementation();

	UE_LOG(LogTemp, Warning, TEXT("in Enemy Die"));

	EnemyState = EEnenyStates::EAS_Dead;

	ClearAttackTimer();
	ShowHealthBar(false);
	DisableCapsule();
	SetLifeSpan(DeathLifeSpam);
	SpawnSoul();
	GetCharacterMovement()->bOrientRotationToMovement = false;
	UE_LOG(LogTemp, Warning, TEXT("Enemy State in Die: %d"), static_cast<int32>(EnemyState));
}

void AEnemy::SpawnSoul()
{
	UWorld* World = GetWorld(); 
	if (World && SoulClass && Attributes)
	{
		//const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 40.f);
		ASoul* SpawnedSoul = World->SpawnActor<ASoul>(SoulClass, GetActorLocation(), GetActorRotation());
		if (SpawnedSoul)
		{
			SpawnedSoul->SetSouls(Attributes->GetSouls());
		}
		
	}

	UE_LOG(LogTemp, Warning, TEXT("Enemy State in SpawnSoul: %d"), static_cast<int32>(EnemyState));
}

void AEnemy::InitializeEnemy()
{
	EnemyController = Cast<AAIController>(GetController());

	ShowHealthBar(false);
	//PatrolTarget = ChoosePatrolTarget();
	MoveTo(PatrolTarget);
	SpawnDefaultWeapon();
}

void AEnemy::CheckPatrolTarget()
{
	if (InRange(PatrolTarget, PatrolRadius))
	{
		UE_LOG(LogTemp, Warning, TEXT("In CheckPatrolTarget"));
		PatrolTarget = ChoosePatrolTarget();
		const float RandWaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, RandWaitTime);

	}

}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LooseInterest();
		if (!isInCombat()) StartPatrolling();

	}
	else if (!IsInAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if (!isInCombat())
		{
			ChaseTarget();
		}
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}

}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;

	//const int32 RandPatrTarget = FMath::RandRange(0, PatrolTargets.Num() - 1);
	//if (RandPatrTarget >= 0)
	//{
	//	return PatrolTargets[RandPatrTarget];
	//}
	//return nullptr;

}

void AEnemy::PatrolTimerFinished()
{
	MoveTo(PatrolTarget);
}

void AEnemy::ShowHealthBar(bool toShow)
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(toShow);
	}
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnenyStates::EAS_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	MoveTo(PatrolTarget);
}

void AEnemy::LooseInterest()
{
	CombatTarget = nullptr;
	ShowHealthBar(false);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InRange(CombatTarget, CombatRadius);
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnenyStates::EAS_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveTo(CombatTarget);
}

bool AEnemy::IsInAttackRadius()
{
	return InRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnenyStates::EAS_Chasing;
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnenyStates::EAS_Attacking;
}

bool AEnemy::isDead()
{
	return EnemyState == EEnenyStates::EAS_Dead;
}

bool AEnemy::isInCombat()
{
	return EnemyState == EEnenyStates::EAS_Combat;
}

void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquipedWeapon = DefaultWeapon;
	}
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnenyStates::EAS_Attacking;
	const float RandTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, RandTime);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::MoveTo(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr)return;
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	EnemyController->MoveTo(MoveRequest);
}

bool AEnemy::InRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;

	//distance from an enemy to the player (magnitude of vector)
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemy::DetectTarget(APawn* PawnSeen)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnenyStates::EAS_Dead &&
		EnemyState != EEnenyStates::EAS_Chasing &&
		EnemyState < EEnenyStates::EAS_Attacking &&
		PawnSeen->ActorHasTag(FName("Player"));

	if (bShouldChaseTarget)
	{
		CombatTarget = PawnSeen;
		GetWorldTimerManager().ClearTimer(PatrolTimer);

		ChaseTarget();
	}
}
