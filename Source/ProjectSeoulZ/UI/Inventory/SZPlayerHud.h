// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/SZQuickSlotDisplay.h"
#include "SZPlayerHud.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-12-26
// Copyright    : 
//
// Description  : Player Hud
//              InGame에서 Interaction하고 QuickSlot을 띄어줌.
//                 
//----------------------------------------------------------------------------------------------------------


UCLASS()
class PROJECTSEOULZ_API USZPlayerHud : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USZQuickSlotDisplay > WB_QuickSlotDisplay;
};
