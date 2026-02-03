// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZEquipmentSlot.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Engine/DataTable.h"

#include "Item/SZItemTemplete.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SZCharacterPlayer.h"
#include "Player/SZPlayerController.h"
#include "Player/Components/SZCharacterEquipmentComponent.h"
#include "UI/Inventory/SZItemTool.h"
#include "UI/Inventory/SZDDO_ItemSlot.h"

void USZEquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();

	ASZPlayerController* SZPC = Cast<ASZPlayerController>(GetOwningPlayer());
	ASZCharacterPlayer* Player = SZPC ? Cast<ASZCharacterPlayer>(SZPC->GetPawn()) : nullptr;
	if (!IsValid(Player))
	{
		Refresh();
		return;
	}

	SZEquipment = Player->GetEquipmentComponent();
	if (!IsValid(SZEquipment) || !SZEquipment->ItemSlots.IsValidIndex(Index))
	{
		Refresh();
		return;
	}

	ItemID = SZEquipment->ItemSlots[Index].ItemID;
	Refresh();
}

FReply USZEquipmentSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (ItemID.IsNone())
	{
		return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	}

	// 우클릭이면 툴팁/상세 표시
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		DisplayItemTool(ItemID);
		return FReply::Handled();
	}

	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

//bool USZEquipmentSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
//{
//	const USZDDO_ItemSlot* DDO = Cast<USZDDO_ItemSlot>(InOperation);
//	if (!DDO || !DDO->SourceInventory || DDO->SourceIndex == INDEX_NONE)
//	{
//		// 드랍 처리 안 함
//		return false; 
//	}
//
//	if (!DDO->SourceInventory->ItemSlots.IsValidIndex(DDO->SourceIndex))
//	{
//		return false;
//	}
//
//	const FItemSlot& ItemSlot = DDO->SourceInventory->ItemSlots[DDO->SourceIndex];
//	const FName DroppedItemID = ItemSlot.ItemID;
//	const int32 DroppedStackCount = ItemSlot.StackCount;
//	// const FItemTemplete* Item = ItemData->FindRow<FItemTemplete>(DroppedItemID, TEXT("USZEquipmentSlot::NativeOnDrop"));
//	// const 
//	const bool bSuccess = CheckItemSlot(DroppedItemID, SlotType);
//	if (!bSuccess)
//	{
//		return true;
//	}
//
//	ItemID = DroppedItemID;
//	if (SZInventoryBase)
//	{
//		SZInventoryBase->AddToNewSlot(DroppedItemID, DroppedStackCount, DDO->SourceIndex);
//		DDO->SourceInventory->RemoveEquippedItem(DDO->SourceIndex, SlotType);
//	}
//	RefreshEquipmentSlot();
//	return true;
//}

void USZEquipmentSlot::DisplayItemTool(FName InItemID)
{
	ASZPlayerController* SZPC = Cast<ASZPlayerController>(GetOwningPlayer());
	if (!SZPC)
	{
		return;
	}

	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(SZPC->GetPawn());
	if (!Player) 
	{
		return;
	}

	SZEquipment = Player->GetEquipmentComponent();
	if (!SZEquipment)
	{
		return;
	}

	if (!ItemToolClass)
	{
		return;
	}

	USZItemTool* ItemToolWidget = CreateWidget<USZItemTool>(SZPC, ItemToolClass);
	ItemToolWidget->ItemID = InItemID;
	ItemToolWidget->SZInventoryBase = SZEquipment;
	// WardrobeUI의 EquipmentSlots의 인덱스로 셋팅
	ItemToolWidget->Index = Index;
	
	ItemToolWidget->AddToViewport();
}

// Wardrobe UI에서 호출하는 함수
void USZEquipmentSlot::RefreshEquip()
{
	Refresh();
}

// Wardrobe UI에서 호출하는 함수
void USZEquipmentSlot::RefreshUnequip()
{
	Refresh();
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

	const FItemTemplete* Item = ItemData->FindRow<FItemTemplete>(ItemID, TEXT("USZEquipmentSlot::Refresh"));
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
