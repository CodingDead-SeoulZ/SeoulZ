// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZQuickSlotDisplay.h"
#include "Player/SZCharacterPlayer.h"
#include "Player/SZPlayerController.h"
#include "Player/Components/SZQuickSlotComponent.h"
#include "UI/Inventory/SZQuickSlotUI.h"
#include "UI/Inventory/SZSelectedSlot.h"

void USZQuickSlotDisplay::NativeConstruct()
{
	Slots.Empty();
	Slots.Reserve(9);

	// TODO. 퀵 슬롯 카테고리 나누기
	Slots.Add(Slot_1);
	Slots.Add(Slot_2);
	Slots.Add(Slot_3);
	Slots.Add(Slot_4);
	// Slots.Add(Slot_5);
	// Slots.Add(Slot_6);
	// Slots.Add(Slot_7);
	// Slots.Add(Slot_8);
	// Slots.Add(Slot_9);

	for (TObjectPtr<USZSelectedSlot> SelectedSlot : Slots)
	{
		if (!IsValid(SelectedSlot))
		{
			continue;
		}

		SelectedSlot->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USZQuickSlotDisplay::UpdateQuickSlot()
{
	ASZPlayerController* SZPC = GetOwningPlayer<ASZPlayerController>();
	if (!IsValid(SZPC)) 
	{
		return;
	}

	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(SZPC->GetPawn());
	if (!IsValid(Player)) 
	{
		return;
	}

	USZQuickSlotComponent* SZQuickSlot = Player->GetComponentByClass<USZQuickSlotComponent>();
	if (!IsValid(SZQuickSlot)) 
	{
		return;
	}

	SZQuickSlot->ItemSlots.SetNum(SZQuickSlot->MaxSlotCount);
	WB_QuickSlotUI->RefreshInventory(SZQuickSlot);
}

void USZQuickSlotDisplay::SetSelectedSlot(int32 Index)
{
	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		if (!IsValid(Slots[i]))
		{
			continue;
		}

		const bool bSelected = (i == Index);
		const ESlateVisibility Vis = bSelected? 
			ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
		Slots[i]->SetVisibility(Vis);
	}
}
