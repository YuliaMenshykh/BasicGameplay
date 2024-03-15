// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

/**
 * 
 */
UCLASS()
class UEBASICS_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
public :

	void SetHealthBarPercent(float Percent);

	void SetStaminaBarPercent(float Percent);

	void SetGold(int Gold);

	void SetSouls(int Souls);

private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressHealthBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressStaminaBar;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoldCountText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SoulsCountText;
};
