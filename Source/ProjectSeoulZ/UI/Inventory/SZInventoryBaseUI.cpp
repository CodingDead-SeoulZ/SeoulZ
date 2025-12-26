// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZInventoryBaseUI.h"
#include "Components/WrapBox.h"
#include "UI/Inventory/SZInventorySlot.h"

void USZInventoryBaseUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	APawn* Player = GetOwningPlayerPawn();
	if (!Player)
	{
		return;
	}

	SZInventory = Player->GetComponentByClass<USZInventoryBaseComponent>();
	if (!SZInventory)
	{
		return;
	}

	CreateInventorySlots();
}

void USZInventoryBaseUI::RefreshInventory(USZInventoryBaseComponent* InSZInventory)
{
	if (!IsValid(InSZInventory))
	{
		return;
	}

	// 동일 타입(USZInventoryComponent) 컴포넌트를 2개 생성(Inventory/QuickSlot)하고,
	// Getter로 구분해 필요한 인스턴스의 포인터를 UI에 전달함.
	SZInventory = InSZInventory;
	CreateInventorySlots();
}

void USZInventoryBaseUI::CreateInventorySlots()
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
				SlotUI->SZInventoryBase = SZInventory;

				WrapBox_Inventory->AddChildToWrapBox(SlotUI);
			}
		}
	}

	SZInventory->OnInventoryUpdated.AddUniqueDynamic(this, &USZInventoryBaseUI::CreateInventorySlots);
}

bool USZInventoryBaseUI::FilterItem(const FItemSlot& InSlot) const
{
	if (!bIsFilterActive)
	{
		return true;
	}

	const FItemTemplete* Item = SZInventory->FindItemData(InSlot.ItemID);
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


bool USZInventoryBaseUI::FilterEmpty(const FItemSlot& InSlot) const
{
	return InSlot.ItemID.IsNone();
}

void USZInventoryBaseUI::OnFilterClicked(bool bIsOn, int32 FilterId)
{
	if (FilterId == 0) 
	{
		return;
	}

	const bool bIsToggled = (CurrentFilter & FilterId) != 0;
	CurrentFilter = bIsToggled ? (CurrentFilter & ~FilterId) : (CurrentFilter | FilterId);
	bIsFilterActive = (CurrentFilter != 0);
	CreateInventorySlots();
}
