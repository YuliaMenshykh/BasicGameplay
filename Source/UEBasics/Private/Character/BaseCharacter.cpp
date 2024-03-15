// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Component/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

ABaseCharacter::ABaseCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ABaseCharacter::isAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}

}

void ABaseCharacter::PlayAttackMontage()
{
	if (AttackMontage)
	{
		const int32 MontageSections = AttackMontage->GetNumSections();
		if (MontageSections <= 0)return;

		const int32 Selection = FMath::RandRange(0, MontageSections - 1);
		PlayMontageSection(AttackMontage, AttackMontage->GetSectionName(Selection));
	}
	
}

void ABaseCharacter::PlayDeathMontage()
{
	UE_LOG(LogTemp, Warning, TEXT("in Base PlayDeathMontage"));
	
	if (DeathMontage)
	{
		
		const int32 MontageSections = DeathMontage->GetNumSections();
		if (MontageSections <= 0)return;
		UE_LOG(LogTemp, Warning, TEXT("in IFFFFF PlayDeathMontage"));
		const int32 Selection = FMath::RandRange(0, MontageSections - 1);
		PlayMontageSection(DeathMontage, DeathMontage->GetSectionName(Selection));
		//for choosing a lay down pose
		SelectedDeathAnimaton = Selection;
	
	}
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::Die_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("in Base Die"));
	Tags.Add(FName("Dead"));
	SetWeaponColisionEnabled(ECollisionEnabled::NoCollision);
	DisableMeshCollision();
	
	PlayDeathMontage();

}

void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionHitReact(const FVector& ImpactPoint)
{

	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0.f)
	{
		Theta *= -1.f;
		//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 60.f, 5.f, FColor::Blue, 5.f);
	}

	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);

	FName Section("FromBack");
	if (Theta >= 2.4 && Theta >= -2.4)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -0.9 && Theta <= -2.4)
	{
		Section = FName("FromRight");
	}
	else if (Theta >= 0.9 && Theta <= 2.4)
	{
		Section = FName("FromLeft");
	}

	PlayHitReactMontage(Section);
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (isAlive() && Hitter)
	{
		DirectionHitReact(Hitter->GetActorLocation());
		PlayHitSound(ImpactPoint);
		PlayHitParticles(ImpactPoint);
		
	}
	else if(!isAlive())
	{
		PlayHitSound(ImpactPoint);
		PlayHitParticles(ImpactPoint);
		Die();
	}


}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint);
	}
}

void ABaseCharacter::PlayHitParticles(const FVector& ImpactPoint)
{
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

FVector ABaseCharacter::GetWarpTargetTranslation()
{
	if (CombatTarget == nullptr) return FVector();

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	return CombatTargetLocation + TargetToMe;

}

FVector ABaseCharacter::GetWarpTargetRotation()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponColisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquipedWeapon && EquipedWeapon->GetWeaponBox())
	{
		EquipedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquipedWeapon->ActorToIgnore.Empty();
	}
}

