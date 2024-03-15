// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/FItem.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class UEBASICS_API ATreasure : public AFItem
{
	GENERATED_BODY()
public:
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
private:

	UPROPERTY(EditAnywhere, Category = "Treasure")
	int32 Gold;

public:

	FORCEINLINE int32 GetGold()const { return Gold; }
};
