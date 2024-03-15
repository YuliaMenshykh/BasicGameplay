// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "Breakable.generated.h"



class UGeometryCollectionComponent;
struct FChaosBreakEvent;

UCLASS()
class UEBASICS_API ABreakable : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakable();

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class ATreasure>>TreasuresToSpawn;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UCapsuleComponent* Capsule;

	UFUNCTION()
	virtual void OnBreake(const FChaosBreakEvent& BreakEvent);

private:

	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	bool bBroken;
	bool bWasSpawned = false;
};
