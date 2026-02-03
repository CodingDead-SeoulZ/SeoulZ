// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZInventoryUI.h"
#include "Components/WrapBox.h"
#include "UI/Inventory/SZInventorySlot.h"
#include "UI/Inventory/SZQuickSlotUI.h"
#include "UI/Inventory/SZItemCategory.h"

void USZInventoryUI::NativeConstruct()
{
	ItemCategorys.Add(WB_ItemCategory);
	ItemCategorys.Add(WB_ItemCategory_1);
	ItemCategorys.Add(WB_ItemCategory_2);

	// Inventory UI 하위 위젯
	WB_QuickSlotUI->FilterAction(ItemCategorys);

	for (USZItemCategory* CategoryWidget : ItemCategorys)
	{
		CategoryWidget->OnFilterClicked.AddDynamic(this, &USZInventoryBaseUI::OnFilterClicked);
	}
}