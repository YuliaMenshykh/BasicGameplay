// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEBASICS_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:	
	
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float CurrentHealth;
		
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost = 20.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenerateRate = 5.f;

public:
	void ReceiveDamage(float Damage);
	void UseStamina(float StaminaCost);
	void RegenerateStamina(float DeltaTime);
	float GetHealthPercent();

	float GetStaminaPercent();

	bool IsAlive();

	void AddGold(int32 GoldAmount);
	void AddSoul(int32 SoulsAmount);

	FORCEINLINE int32 GetGold()const { return Gold; }
	FORCEINLINE int32 GetSouls()const { return Souls; }
	FORCEINLINE float GetDodgeCost() const { return DodgeCost;}
	FORCEINLINE float GetStamina() const { return CurrentStamina; }
};
