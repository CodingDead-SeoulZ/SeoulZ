#pragma once

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	None,
	Pistol,
	Rifle,
	Shotgun,

	Count	UMETA(Hidden)
};

UENUM(BlueprintType, meta = (Bitflags))
enum class EItemCategory : uint8
{
	None        ,
	Weapons     , // 장착템
	Appeal      , // 장착템
	Consumables   // 소모품
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
	Gun,
	Helmet,
	Vest,
	Magazine,
	Gloves,
	Holster,

	Count	UMETA(Hidden)
};
