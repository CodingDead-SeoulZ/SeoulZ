#pragma once

UENUM(BlueprintType, meta = (Bitflags))
enum class EItemCategory : uint8
{
	None        = 0,
	Weapons     = 1 << 0, // 장착템
	Appeal      = 1 << 1, // 장착템
	Consumables = 1 << 2  // 소모품
};
ENUM_CLASS_FLAGS(EItemCategory);

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
	PrimaryGun,
	SecondaryGun,
	Helmet,
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