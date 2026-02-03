// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/SZWardrobeUI.h"
#include "UI/Inventory/SZInventoryUI.h"
#include "UI/Inventory/SZQuickSlotUI.h"
#include "UI/Inventory/SZQuickSlotDisplay.h"
#include "SZPlayerDisplay.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-12-22
// Copyright    : 
//
// Description  : Display 사용자 위젯
//               Inventory UI를 제어
//                 
//----------------------------------------------------------------------------------------------------------

UCLASS()
class PROJECTSEOULZ_API USZPlayerDisplay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	void ShowMouse();

	UFUNCTION(BlueprintCallable)
	void HideMouse();

	UFUNCTION(BlueprintCallable)
	void DisplayInventory();

	UFUNCTION(BlueprintCallable)
	void DisplayQuickSlot();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZWardrobeUI> WB_WardrobeUI;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZInventoryUI> WB_InventoryUI;
};
