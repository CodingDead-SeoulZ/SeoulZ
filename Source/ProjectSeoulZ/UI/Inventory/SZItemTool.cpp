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

#include "Kismet/GameplayStatics.h"
#include "Player/SZCharacterPlayer.h"
#include "Player/Components/SZInventoryBaseComponent.h"
#include "Player/Components/SZInventoryComponent.h"
#include "Player/Components/SZQuickSlotComponent.h"

void USZItemTool::NativeConstruct()
{
	Super::NativeConstruct();

	SetItemToolPose();
	DisplayItemInfo();
	CheckIsEquipment();
	
	if (IsValid(Btn_MoveToQuickSlot))
	{
		CheckMoveToQuickSlot();
		Btn_MoveToQuickSlot->OnItemActionClicked.AddDynamic(this, &USZItemTool::OnMoveToQuickSlot);
	}
	// CheckMoveToQuickSlot();

	// 아이템 사용
	if (IsValid(Btn_Use))
	{
		Btn_Use->OnItemActionClicked.AddDynamic(this, &USZItemTool::OnRequestUseItem);
	}
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
		const FName Row = Item->ItemFragment.CurveDataTableRow;
		const float Level = Item->ItemFragment.Level;

		const float Health = EvalStatFromCurve(Row, Level);
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
			const FName Row = Item->ItemFragment.CurveDataTableRow;
			const float Level = Item->ItemFragment.Level;

			const float Defense = EvalStatFromCurve(Row, Level);
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
			const FName Row = Item->ItemFragment.CurveDataTableRow;
			const float Level = Item->ItemFragment.Level;

			const float Damage = EvalStatFromCurve(Row, Level);
			StatText = FText::Format(
				NSLOCTEXT("SZItemTool", "DamageFmt", "공격력: {0}"),
				FText::AsNumber(Damage));
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

void USZItemTool::CheckMoveToQuickSlot()
{
	if (!IsValid(Btn_MoveToQuickSlot) || !IsValid(Btn_MoveToQuickSlot->Txt_txt))
	{
		return;
	}

	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!IsValid(Player))
	{
		return;
	}

	USZQuickSlotComponent* SZQuickSlot = Player->FindComponentByClass<USZQuickSlotComponent>();
	if (!IsValid(SZQuickSlot))
	{
		return;
	}

	const bool bCheck = (Inventory == SZQuickSlot);
	const FText NewText = bCheck ? 
		FText::FromString(TEXT("퀵 슬롯에서 가져오기")) : FText::FromString(TEXT("퀵 슬롯에 놓기"));

	Btn_MoveToQuickSlot->Txt_txt->SetText(NewText);
}

float USZItemTool::EvalStatFromCurve(FName Row, float Level, float Fallback) const
{
	if (!ItemStats || Row.IsNone())
		return Fallback;

	const FRealCurve* Curve = ItemStats->FindCurve(Row, TEXT("ItemStat"));
	if (!Curve)
		return Fallback;

	return Curve->Eval(Level);
}

void USZItemTool::OnMoveToQuickSlot()
{
	if (!IsValid(Inventory))
	{
		return;
	}

	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!IsValid(Player))
	{
		return;
	}

	USZInventoryComponent* SZInventory = Player->FindComponentByClass<USZInventoryComponent>();
	if (!IsValid(SZInventory))
	{
		return;
	}

	USZQuickSlotComponent* SZQuickSlot = Player->FindComponentByClass<USZQuickSlotComponent>();
	if (!IsValid(SZQuickSlot))
	{
		return;
	}

	const bool bIsNotQuickSlot = (Inventory != SZQuickSlot);
	if (bIsNotQuickSlot)
	{
		// 인벤 -> 핫바
		Inventory->MoveToInventory(SZQuickSlot, Index);
	}
	else
	{
		// 핫바 -> 인벤
		SZQuickSlot->MoveToInventory(SZInventory, Index);
	}

	RemoveFromParent();
}

void USZItemTool::OnRequestUseItem()
{
	if (ItemID.IsNone() || (Index == INDEX_NONE) || !IsValid(Inventory))
	{
		return;
	}

	// 인벤토리한테 아이템 사용 요청
	const bool bSuccess = Inventory->RequestUseItem(ItemID, Index);
	RemoveFromParent();

	if (bSuccess)
	{
		
	}
	else
	{
		// TODO. 사용 불가 표시
	}
}

