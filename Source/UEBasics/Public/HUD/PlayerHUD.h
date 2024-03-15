// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UPlayerOverlay;
/**
 * 
 */
UCLASS()
class UEBASICS_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay()override;
public:

	UPROPERTY(EditDefaultsOnly, Category = Player)
	TSubclassOf<UPlayerOverlay>PlayerOverlayClass;

private:
	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;

public:
	FORCEINLINE UPlayerOverlay* GetPlayerOverlay()const {return PlayerOverlay;}
};
