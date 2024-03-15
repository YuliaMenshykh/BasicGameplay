// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Character/SlashCharacter.h"
#include "Interface/PickupInterface.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* PickupInterf = Cast<IPickupInterface>(OtherActor);
	if (PickupInterf)
	{
		PickupInterf->AddGold(this);
		SpawnPickupSound();
		Destroy();
	}
}
