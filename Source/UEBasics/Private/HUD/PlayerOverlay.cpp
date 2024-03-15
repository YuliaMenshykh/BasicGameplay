// Fill out your copyright notice in the Description page of Project Settings.
#include "HUD/PlayerOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UPlayerOverlay::SetHealthBarPercent(float Percent)
{
	if (ProgressHealthBar)
	{
		ProgressHealthBar->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetStaminaBarPercent(float Percent)
{
	if (ProgressStaminaBar)
	{
		ProgressStaminaBar->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetGold(int Gold)
{
	if (GoldCountText)
	{
		GoldCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Gold)));
	}
}

void UPlayerOverlay::SetSouls(int Souls)
{
	if (SoulsCountText)
	{
		SoulsCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
	}
}
