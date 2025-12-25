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

UCLASS()
class PROJECTSEOULZ_API USZQuickSlotDisplay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateQuickSlot();

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZQuickSlotUI> WB_QuickSlotUI;
};
