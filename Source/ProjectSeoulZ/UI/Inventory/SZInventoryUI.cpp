// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZInventoryUI.h"
#include "Components/WrapBox.h"
#include "UI/Inventory/SZInventorySlot.h"

void USZInventoryUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	APawn* Player = GetOwningPlayerPawn();
	if (!Player) 
	{
		return;
	}

	SZInventory = Player->GetComponentByClass<USZInventoryComponent>();
	if (!SZInventory) 
	{
		return;
	}

	CreateInventorySlots();
}

void USZInventoryUI::RefreshInventory(USZInventoryComponent* InSZInventory)
{
	if (!IsValid(InSZInventory))
	{
		return;
	}

	SZInventory = InSZInventory;
	CreateInventorySlots();
}

void USZInventoryUI::CreateInventorySlots()
{
	if (!IsValid(SZInventory))
	{
		return;
	}

	WrapBox_Inventory->ClearChildren();
	for (int i = 0; i < SZInventory->ItemSlots.Num(); ++i)
	{
		FItemSlot& ItemSlot = SZInventory->ItemSlots[i];
		const bool bPass = FilterItem(ItemSlot) || FilterEmpty(ItemSlot);
		if (bPass)
		{
			if (TObjectPtr<USZInventorySlot> SlotUI = CreateWidget<USZInventorySlot>(GetWorld(), InventorySlotClass)) 
			{
				SlotUI->ItemID = ItemSlot.ItemID;
				SlotUI->StackCount = ItemSlot.StackCount;
				SlotUI->SlotIndex = i;
				SlotUI->SZInventory = SZInventory;

				WrapBox_Inventory->AddChildToWrapBox(SlotUI);
			}
		}
	}

	SZInventory->OnInventoryUpdated.AddUniqueDynamic(this, &USZInventoryUI::CreateInventorySlots);
}

//bool USZInventoryUI::FilterItem(const FItemSlot& InSlot) const
//{
//	if (!bIsFilterActive)
//	{
//		return true;
//	}
//
//	const FItemTemplete* Item = SZInventory->GetItemData(InSlot.ItemID);
//	if (!Item)
//	{
//		return false;
//	}
//
//	const EItemCategory Category = Item->ItemCategory;
//	const int32 Exp = static_cast<int32>(Category) - 1;
//	if (Exp < 0 || Exp >= 31)
//	{
//		return false;
//	}
//
//	const int32 ItemMask = (1 << Exp);
//	return ((ItemMask & CurrentFilter) == CurrentFilter);
//}

bool USZInventoryUI::FilterItem(const FItemSlot& InSlot) const
{
	if (!bIsFilterActive)
	{
		return true;
	}

	const FItemTemplete* Item = SZInventory->GetItemData(InSlot.ItemID);
	if (!Item) 
	{
		return false;
	}

	const EItemCategory Category = Item->ItemCategory;

	// 아이템의 카테고리(enum)를 정수 인덱스로 변환. 예) Weapons = 1, Appeal = 2, Consumables = 3
	const int32 BitIndex = static_cast<int32>(Category);
	if (BitIndex < 0 || BitIndex >= 31)
	{
		return false;
	}

	// 각 카테고리에 해당하는 비트 생성
	// BitIndex = 1 → 0000 0010 (Weapons)
	// BitIndex = 2 → 0000 0100 (Appeal)
	// BitIndex = 3 → 0000 1000 (Consumables)
	const int32 ItemMask = (1 << BitIndex);
	// 즉 선택된 카테고리 중 하나라도 포함되면 true
	return (ItemMask & CurrentFilter) != 0;
}


bool USZInventoryUI::FilterEmpty(const FItemSlot& InSlot) const
{
	return InSlot.ItemID.IsNone();
}
