// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SZWardrobeUI.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-01-05
// Copyright    : 
//
// Description  : 옷장 UI
//              PlayerDisplay의 하위 위젯으로, 의상 및 무기 슬롯을 관리
//                 
//----------------------------------------------------------------------------------------------------------

class USZEquipmentSlot;
class USZInventoryBaseComponent;

UCLASS()
class PROJECTSEOULZ_API USZWardrobeUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Wardrobe")
	void SetEquipment(const FName ItemID, const int32 Index, const int32 EquipmentSlotIndex);
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZEquipmentSlot> Slot_Helmet;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZEquipmentSlot> Slot_Vest;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZEquipmentSlot> Slot_Gloves;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZEquipmentSlot> Slot_Magazine;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZEquipmentSlot> Slot_Holster;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZEquipmentSlot> Slot_PrimaryGun;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZEquipmentSlot> Slot_SubGun;

	UPROPERTY(BlueprintReadOnly, Category = "Wardrobe")
	TArray<TObjectPtr<USZEquipmentSlot>> EquipmentSlots;

private:
	TObjectPtr<USZInventoryBaseComponent> SZInventoryBase;
};
