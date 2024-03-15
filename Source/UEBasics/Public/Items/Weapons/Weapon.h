// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/FItem.h"
#include "Weapon.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class UEBASICS_API AWeapon : public AFItem
{
	GENERATED_BODY()
	
protected:
	
	virtual void BeginPlay() override;

	//virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

	//virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()// in base class for virtual OnSphereOverlap we already have UFUNCTION(), so here i have to add it
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool ActorsAreSame(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& BoxHit);

	//void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

	TArray<AActor*> IgnoreActors;

public:
	AWeapon();

	void Equip(USceneComponent* InParent, FName InSocetName, AActor* NewOwner, APawn* NewInstigator);
	void DeactivateEmbers();
	void DisableSphereCollision();
	void PlayEquipSound();
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocetName);

private:

	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f, 5.f, 5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = false;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

	TArray<AActor*> ActorToIgnore;
};
