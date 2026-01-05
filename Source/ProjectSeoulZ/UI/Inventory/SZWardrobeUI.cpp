// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZWardrobeUI.h"
#include "UI/Inventory/SZEquipmentSlot.h"

#include "Kismet/GameplayStatics.h"
#include "Player/SZCharacterPlayer.h"
#include "Player/Components/SZCharacterEquipmentComponent.h"

void USZWardrobeUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	EquipmentSlots.Reset();
	EquipmentSlots.Reserve(7);

	EquipmentSlots.Add(Slot_Helmet);
	EquipmentSlots.Add(Slot_Vest);
	EquipmentSlots.Add(Slot_Gloves);
	EquipmentSlots.Add(Slot_Magazine);
	EquipmentSlots.Add(Slot_Holster);
	EquipmentSlots.Add(Slot_PrimaryGun);
	EquipmentSlots.Add(Slot_SubGun);
}

void USZWardrobeUI::SetEquipment()
{
	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!IsValid(Player))
	{
		return;
	}

	USZCharacterEquipmentComponent* SZCharacterEquipment = Player->FindComponentByClass<USZCharacterEquipmentComponent>();
	if (!IsValid(SZCharacterEquipment))
	{
		return;
	}

	for (int32 I = 0; I < EquipmentSlots.Num(); ++I)
	{
		USZEquipmentSlot* SlotWidget = EquipmentSlots[I].Get();
		if (!IsValid(SlotWidget))
		{
			continue;
		}

		if (!SZCharacterEquipment->ItemSlots.IsValidIndex(I)) 
		{
			continue;
		}

		const FItemSlot& ItemSlot = SZCharacterEquipment->ItemSlots[I];
		SlotWidget->ItemID = ItemSlot.ItemID; 
		SlotWidget->RefreshEquipmentSlot();
	}
}
