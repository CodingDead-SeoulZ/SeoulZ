// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZQuickSlotDisplay.h"
#include "Player/SZCharacterPlayer.h"
#include "Player/SZPlayerController.h"
#include "Player/Components/SZInventoryComponent.h"
#include "UI/Inventory/SZQuickSlotUI.h"

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

	USZInventoryComponent* SZQuickSlot = Player->GetQuickSlotComponent(); 
	if (!IsValid(SZQuickSlot)) 
	{
		return;
	}

	SZQuickSlot->ItemSlots.SetNum(SZQuickSlot->MaxSlotCount);
	WB_QuickSlotUI->RefreshInventory(SZQuickSlot);
}
