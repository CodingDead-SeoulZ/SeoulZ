// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/SZQuickSlotComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SZPlayerController.h"

void USZQuickSlotComponent::GetSelectedSlot(int32 Index)
{
	if (!ItemSlots.IsValidIndex(Index))
	{
		return;
	}

	const FItemSlot& Slot = ItemSlots[Index];
	const FName ItemID = Slot.ItemID;
	const int32 StackCount = Slot.StackCount;

	ASZPlayerController* SZPC = Cast<ASZPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!SZPC)
	{
		return;
	}

	SZPC->PlayerHUD->WB_QuickSlotDisplay->SetSelectedSlot(Index);
	// Play2DSound();
	UpdateInventory();
}