// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZWardrobeUI.h"
#include "UI/Inventory/SZEquipmentSlot.h"

#include "Kismet/GameplayStatics.h"
#include "Player/SZCharacterPlayer.h"
#include "Player/Components/SZInventoryBaseComponent.h"

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

	SZInventoryBase = Player->FindComponentByClass<USZInventoryBaseComponent>();
	if (!IsValid(SZInventoryBase))
	{
		return;
	}

	SZInventoryBase->OnEquipped.RemoveAll(this);
	SZInventoryBase->OnEquipped.AddUniqueDynamic(this, &USZWardrobeUI::SetEquipment);

	// 옷장 슬롯 델리게이트 수신 직후 테스트
	UE_LOG(LogTemp, Warning, TEXT("[Bind] EquipComp=%s (%p) Bound=%d Owner=%s"),
		*GetNameSafe(SZInventoryBase.Get()), SZInventoryBase.Get(),
		SZInventoryBase ? SZInventoryBase->OnEquipped.IsBound() : 0,
		SZInventoryBase ? *GetNameSafe(SZInventoryBase->GetOwner()) : TEXT("None"));
}

void USZWardrobeUI::SetEquipment(const FName ItemID, const int32 Index, const int32 EquipmentSlotIndex)
{
	UE_LOG(LogTemp, Error, TEXT("QWER"));

	if (ItemID.IsNone() || !EquipmentSlots.IsValidIndex(EquipmentSlotIndex)) 
	{
		return;
	}

	USZEquipmentSlot* SlotWidget = EquipmentSlots[EquipmentSlotIndex].Get();
	if (!IsValid(SlotWidget))
	{
		return;
	}

	if (!SZInventoryBase->ItemSlots.IsValidIndex(Index))
	{
		return;
	}

	const FItemSlot& ItemSlot = SZInventoryBase->ItemSlots[Index];
	SlotWidget->ItemID = ItemSlot.ItemID;
	SlotWidget->RefreshEquipmentSlot();
}
