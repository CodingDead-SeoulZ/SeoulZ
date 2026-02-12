// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Player/Components/SZInventoryBaseComponent.h"
#include "SZInventorySubsystem.generated.h"

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemSlot> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemSlot> QuickSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemSlot> Equipment;
};

class USZInventoryComponent;
class USZQuickSlotComponent;
class USZCharacterEquipmentComponent;

UCLASS()
class PROJECTSEOULZ_API USZInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void Save(
		USZInventoryComponent* InventoryComp,
		USZQuickSlotComponent* QuickSlotComp,
		USZCharacterEquipmentComponent* EquipmentComp
	);

	void Load(
		USZInventoryComponent* InventoryComp,
		USZQuickSlotComponent* QuickSlotComp,
		USZCharacterEquipmentComponent* EquipmentComp
	);

private:
	FInventoryData InventoryData;
};
