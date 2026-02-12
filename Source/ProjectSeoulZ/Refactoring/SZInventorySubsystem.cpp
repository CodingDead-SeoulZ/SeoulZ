// Fill out your copyright notice in the Description page of Project Settings.


#include "Refactoring/SZInventorySubsystem.h"
#include "Player/Components/SZInventoryComponent.h"
#include "Player/Components/SZQuickSlotComponent.h"
#include "Player/Components/SZCharacterEquipmentComponent.h"

void USZInventorySubsystem::Save(USZInventoryComponent* InventoryComp, USZQuickSlotComponent* QuickSlotComp, USZCharacterEquipmentComponent* EquipmentComp)
{
	InventoryData.Inventory = InventoryComp->ItemSlots;
	InventoryData.QuickSlot = QuickSlotComp->ItemSlots;
	InventoryData.Equipment = EquipmentComp->ItemSlots;
}

void USZInventorySubsystem::Load(USZInventoryComponent* InventoryComp, USZQuickSlotComponent* QuickSlotComp, USZCharacterEquipmentComponent* EquipmentComp)
{
	InventoryComp->ItemSlots = InventoryData.Inventory;
	QuickSlotComp->ItemSlots = InventoryData.QuickSlot;
	EquipmentComp->ItemSlots = InventoryData.Equipment;
}
