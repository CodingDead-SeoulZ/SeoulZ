// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZEquipmentSlot.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Engine/DataTable.h"

#include "Item/SZItemTemplete.h"
#include "Player/Components/SZCharacterEquipmentComponent.h"
#include "UI/Inventory/SZDDO_ItemSlot.h"

void USZEquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();

	/*if (Btn_EquipmentSlot)
	{
		Btn_EquipmentSlot->OnClicked.RemoveAll(this);
		Btn_EquipmentSlot->OnClicked.AddDynamic(this, &USZEquipmentSlot::HandleSlotClicked);
	}*/

	Refresh();
}

bool USZEquipmentSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const USZDDO_ItemSlot* DDO = Cast<USZDDO_ItemSlot>(InOperation);
	if (!DDO || !DDO->SourceInventory || DDO->SourceIndex == INDEX_NONE)
	{
		// 드랍 처리 안 함
		return false; 
	}

	if (!DDO->SourceInventory->ItemSlots.IsValidIndex(DDO->SourceIndex))
	{
		return false;
	}

	const FItemSlot& ItemSlot = DDO->SourceInventory->ItemSlots[DDO->SourceIndex];
	const FName DroppedItemID = ItemSlot.ItemID;
	const int32 DroppedStackCount = ItemSlot.StackCount;
	// const FItemTemplete* Item = ItemData->FindRow<FItemTemplete>(DroppedItemID, TEXT("USZEquipmentSlot::NativeOnDrop"));
	// const 
	const bool bSuccess = CheckItemSlot(DroppedItemID, SlotType);
	if (!bSuccess)
	{
		return true;
	}

	ItemID = DroppedItemID;
	if (SZCharacterEquipment)
	{
		SZCharacterEquipment->AddToNewSlot(DroppedItemID, DroppedStackCount, DDO->SourceIndex);
		DDO->SourceInventory->RemoveEquippedItem(DDO->SourceIndex, SlotType);
	}
	RefreshEquipmentSlot();
	return true;
}

void USZEquipmentSlot::SetEmptySlot()
{
	if (Img_Empty)
	{
		Img_Empty->SetBrushFromTexture(EmptySlot, false /*MatchSize*/);
		Img_Empty->SetVisibility(ESlateVisibility::Visible);
	}

	if (Img_Full)
	{
		Img_Full->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USZEquipmentSlot::SetFullSlot(UTexture2D* InTexture)
{
	if (Img_Full)
	{
		Img_Full->SetBrushFromTexture(InTexture, false /*MatchSize*/);
		Img_Full->SetVisibility(ESlateVisibility::Visible);
	}

	if (Img_Empty)
	{
		Img_Empty->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool USZEquipmentSlot::CheckItemSlot(FName InItemID, EEquipmentSlotType InEquipmentSlot) const
{
	if (InItemID.IsNone() || !ItemData)
	{
		return false;
	}

	const FItemTemplete* Item = ItemData->FindRow<FItemTemplete>(InItemID,TEXT("USZEquipmentSlot::CheckItemSlot"));
	if (!Item)
	{
		return false;
	}

	return Item->Equipment.EquipmentSlotType == InEquipmentSlot;
}

void USZEquipmentSlot::Refresh()
{
	if (!Img_Empty || !Img_Full)
	{
		return;
	}

	if (!ItemData || ItemID.IsNone())
	{
		SetEmptySlot();
		return;
	}

	const FItemTemplete* Item = ItemData->FindRow<FItemTemplete>(ItemID, TEXT("USZEquipmentSlot::NativeConstruct"));
	if (!Item)
	{
		SetEmptySlot();
		return;
	}

	UTexture2D* IconTexture = Item->Icon;
	if (!IconTexture)
	{
		SetEmptySlot();
		return;
	}

	SetFullSlot(IconTexture);
}

void USZEquipmentSlot::RefreshEquipmentSlot()
{
	Refresh();
	EquipItem();
}

void USZEquipmentSlot::EquipItem() 
{
	// TODO. 여기서 SZItemTool의 OnRequestUseItem()이 실행되는 게 

}

/*void USZEquipmentSlot::HandleSlotClicked()
{
	// 눌렀을 때 슬롯 타입 전달
	OnSlotSelected.Broadcast(SlotType);
}*/

