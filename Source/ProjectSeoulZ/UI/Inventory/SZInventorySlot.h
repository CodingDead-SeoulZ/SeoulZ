// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/Components/SZInventoryBaseComponent.h"
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
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UFUNCTION(BlueprintCallable)
	void SetItemData();

	UFUNCTION(BlueprintCallable)
	void DisplayItemTool(FName InItemID);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USZItemTool> ItemToolClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USZItemSlotPreview> ItemSlotPreviewClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USZInventoryBaseComponent> SZInventoryBase;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	FName ItemID;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 StackCount;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex;

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
};
