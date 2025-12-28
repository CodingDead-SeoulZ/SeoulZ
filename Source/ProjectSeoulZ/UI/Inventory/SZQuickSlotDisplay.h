// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SZQuickSlotDisplay.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-12-26
// Copyright    : 
//
// Description  : QuickSlot 디스플레이
//              PlayerHud의 하위 위젯
//                 
//----------------------------------------------------------------------------------------------------------


class USZQuickSlotUI;
class USZSelectedSlot;

UCLASS()
class PROJECTSEOULZ_API USZQuickSlotDisplay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateQuickSlot();

	UFUNCTION(BlueprintCallable, Category = "Slot")
	void SetSelectedSlot(int32 Index);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZQuickSlotUI> WB_QuickSlotUI;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZSelectedSlot> Slot_1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZSelectedSlot> Slot_2;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZSelectedSlot> Slot_3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZSelectedSlot> Slot_4;

	/*UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZSelectedSlot> Slot_5;*/

	/*UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZSelectedSlot> Slot_6;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZSelectedSlot> Slot_7;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZSelectedSlot> Slot_8;

	/*UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZSelectedSlot> Slot_9;*/

	UPROPERTY()
	TArray<TObjectPtr<USZSelectedSlot>> Slots;
};
