// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Soul.h"
#include "Interface/PickupInterface.h"
#include "NiagaraFunctionLibrary.h"

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterf = Cast<IPickupInterface>(OtherActor);
	if (PickupInterf)
	{
		PickupInterf->AddSouls(this);
		SpawnPickupEffect();
		SpawnPickupSound();
		Destroy();
	}
	
}

void ASoul::SpawnPickupEffect()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation()
		);
	}
}
