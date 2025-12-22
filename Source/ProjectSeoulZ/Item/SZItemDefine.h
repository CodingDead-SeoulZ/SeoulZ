#pragma once

UENUM(BlueprintType, meta = (Bitflags))
enum class EItemCategory : uint8
{
	None,
	Weapons,	 // 장착템
	Appeal,		 // 장착템
	Consumables // 소모품
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	None,
	// Weapons
	Gun,
	Accessory,
	// Appeal
	Armor,
	Cosmetic,

	Count	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEquipmentSlotType : uint8
{
	None,
	Gun_Primary,
	Gun_Secondary,
	Helemet,
	Vest,
	Magazine,
	Gloves,
	Holster,

	Count	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Common,
	Uncommon,
	Rare,
	Legendary,

	Count	UMETA(Hidden)
};