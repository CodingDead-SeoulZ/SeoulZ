#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"  
#include "SZItemDefine.h"
#include "SZItemTemplete.generated.h"

class ASZItemBase;
class UGameplayEffect;
class USoundBase;
class USkeletalMesh;
class UStaticMesh;
class UMaterialInterface;
class UTexture2D;

USTRUCT(BlueprintType)
struct FItemFragment : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GE;

	UPROPERTY(EditDefaultsOnly) 
	FName CurveDataTableRow = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Level = 0;

	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<UGameplayEffect> DurationGE; // 해당 시간 동안
};

USTRUCT(BlueprintType)
struct FItemEquipment : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentType EquipmentType = EEquipmentType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentSlotType EquipmentSlotType = EEquipmentSlotType::None;
};

USTRUCT(BlueprintType)
struct FItemMesh : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BP_Item")
	TSubclassOf<ASZItemBase> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<USkeletalMesh> SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<UStaticMesh> StaticMesh = nullptr;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<TObjectPtr<UMaterialInterface>> Materials;*/
};

USTRUCT(BlueprintType)
struct FItemSFX : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> Pickup = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> Drop = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> Move = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> Use = nullptr;
};

USTRUCT(BlueprintType)
struct FItemTemplete : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStackCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCategory ItemCategory = EItemCategory::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemFragment ItemFragment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemEquipment Equipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemMesh ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemSFX ItemSFX;
};
