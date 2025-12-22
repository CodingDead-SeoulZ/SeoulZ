// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/Components/SZInventoryComponent.h"
#include "SZInventorySlot.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-12-22
// Copyright    : 
//
// Description  : Slot 사용자 위젯
//               Slot UI를 제어
//                 
//----------------------------------------------------------------------------------------------------------


class UTextBlock;
class UImage;
class UOverlay;
class UButton;
class USZItemTool;

UCLASS()
class PROJECTSEOULZ_API USZInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetItemData();

public:
	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	FName ItemID;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 StackCount;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USZInventoryComponent> SZInventory;

protected:
	// BP에 있는 TextBlock 이름과 동일해야 자동 바인딩됨
	// 텍스트
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_StackCount;

	// 이미지
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img_ItemIcon;

	//오버레이
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_InventorySlot;

	// 버튼
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_InventorySlot;

	// 툴팁은 BP에서 작업
};
