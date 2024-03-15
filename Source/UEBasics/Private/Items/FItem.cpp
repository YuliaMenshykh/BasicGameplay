// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/FItem.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include <UEBasics/DebugMacros.h>
#include "Interface/PickupInterface.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFItem::AFItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh; 


	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	EmbersEffect->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AFItem::BeginPlay()
{
	Super::BeginPlay();
	
	int32 AvgInt = Avg<int32>(1, 3);
	UE_LOG(LogTemp, Warning, TEXT("Avg of 1 and 3 : %d"), AvgInt);
	float AvgFloat = Avg<float>(2.44f, 7.32f);
	UE_LOG(LogTemp, Warning, TEXT("Avg of 1 and 3 : %f"), AvgFloat);
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AFItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AFItem::OnSphereEndOverlap);
}

float AFItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AFItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AFItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterf = Cast<IPickupInterface>(OtherActor);
	if (PickupInterf)
	{
		PickupInterf->SetOverlappingItem(this);
	}
	
}

void AFItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickupInterface* PickupInterf = Cast<IPickupInterface>(OtherActor);
	if (PickupInterf)
	{
		PickupInterf->SetOverlappingItem(nullptr);
	}
	
}

void AFItem::SpawnPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			PickupSound,
			GetActorLocation()
		);
	}
}

// Called every frame
void AFItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
	
	
}

