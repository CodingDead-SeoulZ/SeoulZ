// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZItemTool.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/Inventory/SZItemActionUI.h"
#include "UI/Inventory/SZItemSlider.h"
#include "Components/Border.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

#include "Kismet/GameplayStatics.h"
#include "Player/SZCharacterPlayer.h"
#include "Player/SZPlayerController.h"
#include "Player/Components/SZInventoryBaseComponent.h"
#include "Player/Components/SZInventoryComponent.h"
#include "Player/Components/SZQuickSlotComponent.h"
#include "Player/Components/SZCharacterEquipmentComponent.h"

void USZItemTool::NativeConstruct()
{
	Super::NativeConstruct();

	SetItemToolPose();
	DisplayItemInfo();
	DisplayButtonUI();
	CheckIsEquipment();

	if (IsValid(Btn_MoveToQuickSlot))
	{
		CheckMoveToQuickSlot();
		Btn_MoveToQuickSlot->OnItemActionClicked.AddDynamic(this, &USZItemTool::OnMoveToQuickSlot);
	}

	if (IsValid(Btn_Use))
	{
		bool bCheck = CheckMoveToEquipmentSlot();
		(bCheck)? Btn_Use->OnItemActionClicked.AddDynamic(this, &USZItemTool::OnRequestUnequipItem) : Btn_Use->OnItemActionClicked.AddDynamic(this, &USZItemTool::OnRequestUseItem);
	}

	if (IsValid(Btn_Drop)) 
	{
		Btn_Drop->OnItemActionClicked.AddDynamic(this, &USZItemTool::OnDropSlider);
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

	if (Item->ItemAmmo.AmmoType != EAmmoType::None) 
	{
		if (Btn_Use)
		{
			Btn_Use->SetVisibility(ESlateVisibility::Hidden);
		}
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

void USZItemTool::DisplayButtonUI()
{
	if (!IsValid(Btn_MoveToQuickSlot) || !IsValid(Btn_Drop))
	{
		return;
	}

	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!IsValid(Player))
	{
		return;
	}

	USZCharacterEquipmentComponent* SZEquipment = Player->GetEquipmentComponent();
	if (!IsValid(SZEquipment))
	{
		return;
	}

	const bool bCheck = (SZInventoryBase == SZEquipment);
	if (bCheck)
	{
		// 장착
		Btn_MoveToQuickSlot->SetVisibility(ESlateVisibility::Hidden);
		Btn_Drop->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	else
	{
		Btn_MoveToQuickSlot->SetVisibility(ESlateVisibility::Visible);
		Btn_Drop->SetVisibility(ESlateVisibility::Visible);
		return;
	}
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

	USZQuickSlotComponent* SZQuickSlot = Player->GetQuickSlotComponent();
	if (!IsValid(SZQuickSlot))
	{
		return;
	}

	const bool bCheck = (SZInventoryBase == SZQuickSlot);
	const FText NewText = bCheck ? 
		FText::FromString(TEXT("퀵 슬롯에서 가져오기")) : FText::FromString(TEXT("퀵 슬롯에 놓기"));

	Btn_MoveToQuickSlot->Txt_txt->SetText(NewText);
}

bool USZItemTool::CheckMoveToEquipmentSlot()
{
	if (!IsValid(Btn_Use) || !IsValid(Btn_Use->Txt_txt))
	{
		return false;
	}

	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!IsValid(Player))
	{
		return false;
	}

	USZCharacterEquipmentComponent* SZEquipment = Player->GetEquipmentComponent();
	if (!IsValid(SZEquipment))
	{
		return false;
	}

	const bool bCheck = (SZInventoryBase == SZEquipment);
	const FText NewText = bCheck ?
		FText::FromString(TEXT("장착 해제하기")) : FText::FromString(TEXT("사용하기"));
	
	Btn_Use->Txt_txt->SetText(NewText);

	return bCheck;
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

void USZItemTool::OnDropSlider()
{
	if (!WBItemSliderClass || !IsValid(SZInventoryBase))
	{
		return;
	}

	ASZPlayerController* SZPC = Cast<ASZPlayerController>(GetOwningPlayer());
	if (!SZPC)
	{
		return;
	}

	USZItemSlider* ItemSlider = CreateWidget<USZItemSlider>(SZPC, WBItemSliderClass);
	if (!ItemSlider)
	{
		return;
	}

	ItemSlider->SZInventoryBase = SZInventoryBase;
	ItemSlider->Index = Index;

	WBItemSlider = ItemSlider;
	WBItemSlider->AddToViewport(0);
}

void USZItemTool::OnMoveToQuickSlot()
{
	if (!IsValid(SZInventoryBase))
	{
		return;
	}

	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!IsValid(Player))
	{
		return;
	}

	USZInventoryComponent* SZInventory = Player->GetInventoryComponent();
	if (!IsValid(SZInventory))
	{
		return;
	}

	USZCharacterEquipmentComponent* SZEquipment = Player->GetEquipmentComponent();
	if (!IsValid(SZEquipment))
	{
		return;
	}

	USZQuickSlotComponent* SZQuickSlot = Player->GetQuickSlotComponent();
	if (!IsValid(SZQuickSlot))
	{
		return;
	}

	const bool bIsNotQuickSlot = (SZInventoryBase != SZQuickSlot);
	if (bIsNotQuickSlot)
	{
		// 인벤 -> 핫바
		SZInventoryBase->MoveToInventory(SZQuickSlot, Index);
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
	if (ItemID.IsNone() || (Index == INDEX_NONE) || !IsValid(SZInventoryBase))
	{
		return;
	}

	// 인벤토리한테 아이템 사용 요청
	const bool bSuccess = SZInventoryBase->RequestUseItem(ItemID, Index);
	RemoveFromParent();

	if (bSuccess)
	{
		
	}
	else
	{
		// TODO. 사용 불가 표시
	}
}

void USZItemTool::OnRequestUnequipItem()
{
	// 현재 문제 상황.
	// ItemTool의 OnRequestUnequipItem() 함수에서 EquipmentComponent(파생)의 GEHandles가 Empty

	// 문제 원인.
	// Inventory가 부모 Base가 아닌, 파생 Equipment로 들어와서? 그 과정에서 GEHandles가 Empty
	// SZInventorySlot에서 SZInventoryBase(부모)를 SZInventoryBase(부모)로 셋팅해서 발생한 문제. 그런데 본 셋팅을 변경할 수 없는 노릇.
	// 그렇다면 GEHandles를 어떻게 전달해주지?

	// 해결 방안.
	// Player에서 GEHandles를 관리

	if (ItemID.IsNone() || (Index == INDEX_NONE) || !IsValid(SZInventoryBase))
	{
		return;
	}

	const FItemTemplete* Item = ItemData->FindRow<FItemTemplete>(ItemID, TEXT("USZItemTool::OnRequestUnequipItem"));
	if (!Item)
	{
		return;
	}

	if (Item->ItemCategory == EItemCategory::Appeal)
	{
		// 인벤토리한테 아이템(의상) 해제 요청
		const bool bSuccess = SZInventoryBase->RequestUnequipItem(ItemID, Index);
	}
	else if (Item->ItemCategory == EItemCategory::Weapons)
	{
		// 인벤토리한테 아이템(무기) 해제 요청
		const bool bSuccess = SZInventoryBase->RequestUnequipWeaponItem(ItemID, Index);
	}
	
	RemoveFromParent();
	return;
}