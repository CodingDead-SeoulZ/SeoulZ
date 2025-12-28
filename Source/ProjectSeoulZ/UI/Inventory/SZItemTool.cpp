// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZItemTool.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/Inventory/SZItemActionUI.h"
#include "Components/Border.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Item/SZItemTemplete.h"

void USZItemTool::NativeConstruct()
{
	Super::NativeConstruct();

	SetItemToolPose();
	DisplayItemInfo();
	CheckIsEquipment();
}

void USZItemTool::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	RemoveFromParent();
	Super::NativeOnMouseLeave(InMouseEvent);
}

void USZItemTool::SetItemToolPose()
{
	if (!Border_ItemTool)
	{
		return;
	}

	const FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Border_ItemTool->Slot))
	{
		CanvasSlot->SetPosition(MousePos);
	}
}

void USZItemTool::DisplayItemInfo()
{
	if (!ItemData || ItemID.IsNone())
	{
		return;
	}

	const FItemTemplete* Item = ItemData->FindRow<FItemTemplete>(ItemID, TEXT("USZItemTool::DisplayItemInfo"));
	if (!Item)
	{
		return;
	}

	if (Txt_ItemName)
	{
		Txt_ItemName->SetText(Item->Name);
	}

	if (Txt_ItemDescription)
	{
		Txt_ItemDescription->SetText(Item->Description);
	}

	if (Img_Icon)
	{
		Img_Icon->SetBrushFromTexture(Item->Icon, true);
	}
}

void USZItemTool::DisplayItemStat()
{
	if (!ItemData || ItemID.IsNone() || !Txt_ItemStat)
	{
		return;
	}

	const FItemTemplete* Item = ItemData->FindRow<FItemTemplete>(ItemID, TEXT("USZItemTool::DisplayItemStat"));
	if (!Item)
	{
		return;
	}

	FText StatText = FText::GetEmpty();
	switch (Item->ItemCategory)
	{
	case EItemCategory::Consumables:
	{
		const int32 Health = Item->Consumables.HealthAmount;
		StatText = FText::Format(
			NSLOCTEXT("SZItemTool", "ConsumableHealthFmt", "체력: {0}"),
			FText::AsNumber(Health)
		);
		break;
	}
	case EItemCategory::Appeal:
	{
		const bool bArmor = (Item->Equipment.EquipmentType == EEquipmentType::Armor);
		if (bArmor)
		{
			const int32 Defense = Item->Equipment.Defense;
			StatText = FText::Format(
				NSLOCTEXT("SZItemTool", "DefenseFmt", "방어력: {0}"), 
				FText::AsNumber(Defense)
			);
		}
		break;
	}
	case EItemCategory::Weapons:
	{
		const bool bGun = (Item->Equipment.EquipmentType == EEquipmentType::Gun);
		const bool bAccessory = (Item->Equipment.EquipmentType == EEquipmentType::Accessory);
		if (bGun)
		{
			StatText = FText::Format(
				NSLOCTEXT("SZItemTool", "DamageFmt", "공격력: {0}"),
				FText::AsNumber(Item->Equipment.Damage));
		}
		//else if (bAccessory)
		//{
		//	// TODO. 
		//	StatText = FText::FromString(TEXT("공격력 없음"));
		//}
		break;
	}
	default:
		break;
	}

	Txt_ItemStat->SetText(StatText);
}

void USZItemTool::CheckIsEquipment()
{
	if (!ItemData || ItemID.IsNone())
	{
		return;
	}

	const FItemTemplete* Item = ItemData->FindRow<FItemTemplete>(ItemID, TEXT("USZItemTool::DisplayItemInfo"));
	if (!Item)
	{
		return;
	}

	// 1) 스택 칸
	if (VBox_Stats) 
	{
		// TODO. Item Templete 데이터 구조 변경하기
		DisplayItemStat();
	}
	
	// TODO. 효과 칸
	// 임시로 보이지 않게 처리
	if (VBox_Effects)
	{
		VBox_Effects->SetVisibility(ESlateVisibility::Hidden);
	} 
}
