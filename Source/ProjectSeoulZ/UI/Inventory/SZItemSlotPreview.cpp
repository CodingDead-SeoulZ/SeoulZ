// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZItemSlotPreview.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/SZItemTemplete.h"

void USZItemSlotPreview::NativePreConstruct()
{
	const FItemTemplete* Item = ItemTable ? ItemTable->FindRow<FItemTemplete>(ItemID, TEXT("Find Item")) : nullptr;
	if (!Item) 
	{
		return;
	}

	Img_Item->SetBrushFromTexture(Item->Icon, true);
	Txt_StackCount->SetText(FText::AsNumber(StackCount));
}
