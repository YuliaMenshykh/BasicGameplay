// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class UEBASICS_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:

	//name has to match name in WBP_HealthBar
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
};
