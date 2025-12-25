// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/SZInventoryBaseUI.h"
#include "SZInventoryUI.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-12-22
// Copyright    : 
//
// Description  : Inventory Base를 상속받는 Inventory 사용자 위젯
//              하위 위젯(카테고리 위젯)에서 상태 변경이 발생하면, 델리게이트를 통해 이 OnFilterClicked()를 호출.
//                 
//----------------------------------------------------------------------------------------------------------

class USZQuickSlotUI;
class USZItemCategory;

UCLASS()
class PROJECTSEOULZ_API USZInventoryUI : public USZInventoryBaseUI
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZQuickSlotUI> WB_QuickSlotUI;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZItemCategory> WB_ItemCategory;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZItemCategory> WB_ItemCategory_1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZItemCategory> WB_ItemCategory_2;

	UPROPERTY()
	TArray<TObjectPtr<USZItemCategory>> ItemCategorys;
};
