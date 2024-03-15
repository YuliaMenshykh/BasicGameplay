// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/FItem.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class UEBASICS_API ASoul : public AFItem
{
	GENERATED_BODY()
	
protected:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

	void SpawnPickupEffect();

private:

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;

	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	int32 Souls;

public:
	FORCEINLINE int32 GetSouls()const { return Souls; }
	FORCEINLINE void SetSouls(int32 SoulsAmount) { Souls = SoulsAmount; }
};
