#pragma once

UENUM(BlueprintType)
enum class ECharacterStates : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "EquippedOneHandedWeapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "EquippedTwoHandedWeapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EEnenyStates : uint8
{
	EAS_NoState UMETA(DisplayName = "NoState"),
	EAS_Dead UMETA(DisplayName = "Dead"),
	EAS_Patrolling UMETA(DisplayName = "Patrolling"),
	EAS_Chasing UMETA(DisplayName = "Chasing"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Combat UMETA(DisplayName = "Combat"),

};
