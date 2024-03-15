// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0, MaxHealth);
}

void UAttributeComponent::UseStamina(float StaminaCost)
{
	CurrentStamina = FMath::Clamp(CurrentStamina - StaminaCost, 0, MaxStamina);
}

void UAttributeComponent::RegenerateStamina(float DeltaTime)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRegenerateRate * DeltaTime, 0, MaxStamina);
}

float UAttributeComponent::GetHealthPercent()
{
	return CurrentHealth/MaxHealth;
}

float UAttributeComponent::GetStaminaPercent()
{
	return CurrentStamina / MaxStamina;
}

bool UAttributeComponent::IsAlive()
{
	return CurrentHealth > 0.f;
}

void UAttributeComponent::AddGold(int32 GoldAmount)
{
	Gold += GoldAmount;
}

void UAttributeComponent::AddSoul(int32 SoulsAmount)
{
	Souls += SoulsAmount;
}


void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

