// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Inventory/SZInventoryBaseUI.h"
#include "SZQuickSlotUI.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-12-26
// Copyright    : 
//
// Description  : Inventory Base를 상속받는 QuickSlot 사용자 위젯
//              - Inventory UI의 하위 위젯으로 카테고리 변경 시 델리게이트로 변경사항 전달
//              - QuickSlot의 하위 위젯
//                 
//----------------------------------------------------------------------------------------------------------

class USZItemCategory;

UCLASS()
class PROJECTSEOULZ_API USZQuickSlotUI : public USZInventoryBaseUI
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void FilterAction(const TArray<USZItemCategory*>& ItemCategorys);
};
