// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZItemSlider.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "Player/Components/SZInventoryBaseComponent.h"

void USZItemSlider::NativeConstruct()
{
    Super::NativeConstruct();

    SetItemSliderPosition();
    SetSliderValue();
    SetSlotStack();

    if (IsValid(Slider_ItemStack))
    {
        Slider_ItemStack->OnValueChanged.AddDynamic(this, &USZItemSlider::OnSliderValueChanged);
    }

    if (IsValid(Btn_Cancel))
    {
        Btn_Cancel->OnClicked.AddDynamic(this, &USZItemSlider::OnClickedCancel);
    }

    if (IsValid(Btn_Drop))
    {
        Btn_Drop->OnClicked.RemoveDynamic(this, &USZItemSlider::OnClickedDrop);
        Btn_Drop->OnClicked.AddDynamic(this, &USZItemSlider::OnClickedDrop);
    }
}

void USZItemSlider::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);

    RemoveFromParent();
}

int32 USZItemSlider::GetItemStack() const
{
    if (!SZInventoryBase) 
    {
        return 0;
    }

    if (!SZInventoryBase->ItemSlots.IsValidIndex(Index))
    {
        return 0;
    }

    const FItemSlot& ItemSlot = SZInventoryBase->ItemSlots[Index];
    return ItemSlot.StackCount;
}

void USZItemSlider::SetSliderValue()
{
    if (!IsValid(Txt_ItemStack))
    {
        return;
    }

    const int32 Stack = GetItemStack();
    Txt_ItemStack->SetText(FText::AsNumber(Stack));
}

void USZItemSlider::SetSlotStack()
{
    if (!IsValid(Slider_ItemStack))
    {
        return;
    }

    const float Stack = static_cast<float>(GetItemStack());
    Slider_ItemStack->SetValue(Stack);
    Slider_ItemStack->SetMaxValue(Stack);
}

void USZItemSlider::OnSliderValueChanged(float Value)
{
    ItemStack = FMath::TruncToInt(Value);

    if (IsValid(Txt_ItemStack))
    {
        Txt_ItemStack->SetText(FText::AsNumber(ItemStack));
    }
}

void USZItemSlider::OnClickedCancel()
{
    RemoveFromParent();
}

void USZItemSlider::OnClickedDrop()
{
    if (!IsValid(SZInventoryBase) || Index == INDEX_NONE)
    {
        return;
    }

    const int32 DropStack = FMath::Max(1, ItemStack);
    SZInventoryBase->RemoveFromInventory(Index, DropStack);
}

void USZItemSlider::SetItemSliderPosition()
{
    if (!IsValid(Border_Slider))
    {
        return;
    }

    UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Border_Slider->Slot);
    if (!CanvasSlot)
    {
        return;
    }

    FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
    if (MousePos.IsNearlyZero())
    {
        return;
    }
    CanvasSlot->SetPosition(MousePos);
}
