// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 * 
 */
UCLASS()
class UEBASICS_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

	void SetHealthPercent(float Percent);

private:

	//UPROPERTY make shure that it doesn't start off initialized with garbage data
	UPROPERTY()
	class UHealthBar* HealthBarWidget;
};
