// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FItem.generated.h"

class USphereComponent;

enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

UCLASS()
class UEBASICS_API AFItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	float Amplitude = 0.25f;
	UPROPERTY(EditInstanceOnly)
	float TimeConstant = 5.f;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransformedCos();

	template<typename T>
	T Avg(T First, T Second);

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void SpawnPickupSound();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

	EItemState ItemState = EItemState::EIS_Hovering;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* EmbersEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;

private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

template<typename T>
inline T AFItem::Avg(T First, T Second)
{
	return (First+Second) /2;
}
