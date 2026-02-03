// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZQuickSlotUI.h"
#include "UI/Inventory/SZItemCategory.h"

void USZQuickSlotUI::FilterAction(const TArray<USZItemCategory*>& ItemCategorys)
{
	for (USZItemCategory* CategoryWidget : ItemCategorys)
	{
		CategoryWidget->OnFilterClicked.AddDynamic(this, &USZInventoryBaseUI::OnFilterClicked);
	}
}