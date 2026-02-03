// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/SZItemDefine.h"
#include "SZEquipmentSlot.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-01-05
// Copyright    : 
//
// Description  : 옷장 슬롯 UI
//              1.  EquipmentSlot Icon문제
//				1 - 1. ItemID가 None으로 초기화되는 문제 -> 해결 완료
//				1 - 2. EquipmentSlot은 비워지고, InventorySlot에 채워지게 -> 해결완료
//				
//				2. EquipmentSlot에서 상세보기 변경하기 -> 해결 완료
//                 
//----------------------------------------------------------------------------------------------------------

class USZItemTool;
class USZCharacterEquipmentComponent;
class UButton;
class UBorder;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentSlotClicked);

UCLASS()
class PROJECTSEOULZ_API USZEquipmentSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION(BlueprintCallable)
	void DisplayItemTool(FName InItemID);

	UFUNCTION(BlueprintCallable, Category = "EquipmentSlot")
	void RefreshEquip();

	UFUNCTION(BlueprintCallable, Category = "EquipmentSlot")
	void RefreshUnequip();

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "EquipmentSlot")
	void SetEmptySlot();

	UFUNCTION(BlueprintCallable, Category = "EquipmentSlot")
	void SetFullSlot(UTexture2D* InTexture);

	UFUNCTION(BlueprintCallable, Category = "EquipmentSlot")
	bool CheckItemSlot(FName InItemID, EEquipmentSlotType InEquipmentSlot) const;

	UFUNCTION(BlueprintCallable, Category = "EquipmentSlot")
	void Refresh();

public:
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentSlotClicked OnEquipmentSlotClicked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentSlot", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USZItemTool> ItemToolClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentSlot", meta = (AllowPrivateAccess = "true"))
	int32 Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentSlot", meta = (AllowPrivateAccess = "true"))
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentSlot", meta = (AllowPrivateAccess = "true"))
	EEquipmentSlotType SlotType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentSlot", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTexture2D> EmptySlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	TObjectPtr<UDataTable> ItemData;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<USZCharacterEquipmentComponent> SZEquipment;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Btn_EquipmentSlot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> Border_Outer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Img_Empty;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Img_Full;
};
