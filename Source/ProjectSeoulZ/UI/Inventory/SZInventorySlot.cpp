// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZInventorySlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "UI/Inventory/SZItemTool.h"
#include "Player/SZPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Inventory/SZItemSlotPreview.h"
#include "UI/Inventory/SZDDO_ItemSlot.h"

void USZInventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	SetItemData();
}

FReply USZInventorySlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

	// 좌클릭이면 드래그 시작 신호
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		const FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return EventReply.NativeReply;
	}

	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

void USZInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// 아이템 이동 위젯 생성
	USZItemSlotPreview* PreviewWidget = CreateWidget<USZItemSlotPreview>(GetOwningPlayer(), ItemSlotPreviewClass);
	if (!PreviewWidget)
	{
		return;
	}
	PreviewWidget->ItemID = ItemID;
	PreviewWidget->StackCount = StackCount;

	// 
	USZDDO_ItemSlot* DragOp = NewObject<USZDDO_ItemSlot>();
	DragOp->DefaultDragVisual = PreviewWidget;
	DragOp->Pivot = EDragPivot::CenterCenter;
	// 어느 인벤토리에서 왔는지
	DragOp->SourceInventory = SZInventoryBase;
	DragOp->SourceIndex = SlotIndex;

	OutOperation = DragOp;
}

bool USZInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	USZDDO_ItemSlot* DragOp = Cast<USZDDO_ItemSlot>(InOperation);
	if (!DragOp)
	{
		return false;
	}

	// 자기 자신 드래그앤드랍은 무시
	if (DragOp->SourceInventory == SZInventoryBase && DragOp->SourceIndex == SlotIndex)
	{
		return false;
	}
    
	// 아이템 이동 함수 호출
	SZInventoryBase->TransferSlots(SlotIndex, DragOp->SourceIndex, DragOp->SourceInventory);
	return true;
}

void USZInventorySlot::SetItemData()
{
	if (!IsValid(SZInventoryBase)) 
	{
		return;
	}

	const FItemTemplete* Item = SZInventoryBase->FindItemData(ItemID);
	if (!Item) 
	{
		Overlay_InventorySlot->SetVisibility(ESlateVisibility::Collapsed);
	}
	else 
	{
		TObjectPtr<UTexture2D> Icon = SZInventoryBase->FindItemData(ItemID)->Icon;
		if (Icon)
		{
			Img_ItemIcon->SetBrushFromTexture(Icon);
		}

		Txt_StackCount->SetText(FText::AsNumber(StackCount));
		Overlay_InventorySlot->SetVisibility(ESlateVisibility::Visible);
	}
}

void USZInventorySlot::DisplayItemTool(FName InItemID)
{
	ASZPlayerController* SZPC = Cast<ASZPlayerController>(GetOwningPlayer());
	if (!SZPC) 
	{
		return;
	}

	if (!ItemToolClass)
	{
		return;
	}

	USZItemTool* ItemToolWidget = CreateWidget<USZItemTool>(SZPC, ItemToolClass);
	ItemToolWidget->ItemID = InItemID;
	ItemToolWidget->Inventory = SZInventoryBase;
	ItemToolWidget->Index = SlotIndex;
	ItemToolWidget->AddToViewport();
}


