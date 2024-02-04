// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/Breakable.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Treasure.h"
#include "Components/CapsuleComponent.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"

ABreakable::ABreakable()
{
	PrimaryActorTick.bCanEverTick = false;
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GeometryCollection->SetNotifyBreaks(true);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule component"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Block);
}

void ABreakable::BeginPlay()
{
	Super::BeginPlay();
	GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakable::OnBreake);
}

void ABreakable::OnBreake(const FChaosBreakEvent& BreakEvent)
{
	this->SetLifeSpan(3.f);

	UWorld* World = GetWorld();
	if (World && TreasuresToSpawn.Num() > 0, bWasSpawned == false)
	{
		bWasSpawned = true;
		int32 Selection = FMath::RandRange(0, TreasuresToSpawn.Num() - 1);
		World->SpawnActor<ATreasure>(TreasuresToSpawn[Selection], FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 100.f), GetActorRotation());
	}
}

// Called every frame
void ABreakable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakable::GetHit(const FVector& ImpactPoint)
{
	if (bBroken)return;
	bBroken = true;

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint);
	}
	
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	
}

