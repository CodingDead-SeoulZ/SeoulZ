// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZWardrobeUI.h"
#include "UI/Inventory/SZEquipmentSlot.h"

#include "Kismet/GameplayStatics.h"
#include "Player/SZCharacterPlayer.h"
#include "Player/Components/SZInventoryBaseComponent.h"
#include "Player/Components/SZCharacterEquipmentComponent.h"

void USZWardrobeUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	EquipmentSlots.Reset();
	EquipmentSlots.Reserve(7);

	EquipmentSlots.Add(Slot_Helmet);
	EquipmentSlots.Add(Slot_Vest);
	EquipmentSlots.Add(Slot_Gloves);
	EquipmentSlots.Add(Slot_Magazine);
	EquipmentSlots.Add(Slot_Holster);
	EquipmentSlots.Add(Slot_PrimaryGun);
	EquipmentSlots.Add(Slot_SubGun);
}

void USZWardrobeUI::NativeConstruct()
{
	Super::NativeConstruct();

	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!IsValid(Player))
	{
		return;
	}
	
	// TODO. ★ 델리게이트 문제
	// https://chatgpt.com/share/e/695cdc12-aa64-8002-bd72-3e846cce8d3d
	// 클래스 상속 관계 및 FindComponentByClass()가 정확히 어떤 컴포넌트를 어떻게 반환하는지. 
	// 가장 첫 번째꺼? Player에서 접근하는 것이므로 파생 중 가장 첫 번째꺼인지. 아니면 정말 Base인지. 
	SZInventoryBase = Player->FindComponentByClass<USZInventoryBaseComponent>();
	if (!IsValid(SZInventoryBase))
	{
		return;
	}

	// 델리게이트 수신. 아이템 장착
	SZInventoryBase->OnEquipped.RemoveAll(this);
	SZInventoryBase->OnEquipped.AddUniqueDynamic(this, &USZWardrobeUI::SetEquipment);

	// 델리게이트 수신. 아이템 해제
	USZCharacterEquipmentComponent* SZEquipment = Player->GetEquipmentComponent();
	if (!SZEquipment)
	{
		return;
	}

	SZEquipment->OnUnequipped.RemoveAll(this);
	SZEquipment->OnUnequipped.AddUniqueDynamic(this, &USZWardrobeUI::ClearEquipment);
}

void USZWardrobeUI::SetEquipment(const FName ItemID, const int32 Index, const int32 EquipmentSlotIndex)
{
	if (ItemID.IsNone() || !EquipmentSlots.IsValidIndex(EquipmentSlotIndex)) 
	{
		return;
	}

	USZEquipmentSlot* SlotWidget = EquipmentSlots[EquipmentSlotIndex].Get();
	if (!IsValid(SlotWidget))
	{
		return;
	}

	if (!IsValid(SZInventoryBase))
	{
		return;
	}

	if (!SZInventoryBase->ItemSlots.IsValidIndex(Index))
	{
		return;
	}

	const FItemSlot& ItemSlot = SZInventoryBase->ItemSlots[Index];
	SlotWidget->ItemID = ItemSlot.ItemID;
	SlotWidget->RefreshEquip();
}

void USZWardrobeUI::ClearEquipment(const int32 EquipmentSlotIndex)
{
	if (!EquipmentSlots.IsValidIndex(EquipmentSlotIndex))
	{
		return;
	}

	USZEquipmentSlot* SlotWidget = EquipmentSlots[EquipmentSlotIndex].Get();
	if (!IsValid(SlotWidget))
	{
		return;
	}

	if (!IsValid(SZInventoryBase))
	{
		return;
	}

	const FItemSlot& ItemSlot = SZInventoryBase->ItemSlots[EquipmentSlotIndex];
	SlotWidget->ItemID = ItemSlot.ItemID;
	SlotWidget->RefreshUnequip();
}
