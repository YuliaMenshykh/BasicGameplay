// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/CharacterTypes.h"
#include "SlashAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class UEBASICS_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public :
	virtual void NativeInitializeAnimation()override;//Event Blueprint Initialize Animation

	virtual void NativeUpdateAnimation(float DeltaTime)override;//Event Blueprint Update Animation

	UPROPERTY(BlueprintReadOnly)
	class ASlashCharacter* SlashCharacter;

	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* SlashCharacterMovement;

	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly)
	ECharacterStates CharacterState;

	UPROPERTY(BlueprintReadOnly)
	EActionState ActionState;

	UPROPERTY(BlueprintReadOnly)
	int SelectedDeathAnimaton;
};
