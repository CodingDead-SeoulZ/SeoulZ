// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/SZItemTemplete.h"
#include "SZItemTool.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-12-27
// Copyright    : 
//
// Description  : Item Tool
//               아이템 상세 보기
//                 
//----------------------------------------------------------------------------------------------------------

class USZInventoryBaseComponent;
class UVerticalBox;
class UTextBlock;
class UImage;
class USZItemActionUI;
class UBorder;

UCLASS()
class PROJECTSEOULZ_API USZItemTool : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	void SetItemToolPose();
	void DisplayItemInfo();
	void DisplayItemStat();
	void DisplayButtonUI();
	void CheckIsEquipment();
	void CheckMoveToQuickSlot();

	bool CheckMoveToEquipmentSlot();

	float EvalStatFromCurve(FName Row, float Level, float Fallback = 0.f) const;

	UFUNCTION()
	void OnMoveToQuickSlot();

	UFUNCTION()
	void OnRequestUseItem();

	UFUNCTION()
	void OnRequestUnequipItem();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	TObjectPtr<UDataTable> ItemData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Curve Data")
	TObjectPtr<UCurveTable> ItemStats;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	FName ItemID;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (ExposeOnSpawn = "true"))
	int32 Index = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<USZInventoryBaseComponent> SZInventoryBase;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> VBox_Stats;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> VBox_Effects;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_ItemName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_ItemStat;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_ItemEffects;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_ItemDescription;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Img_Icon;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZItemActionUI> Btn_Use;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZItemActionUI> Btn_MoveToQuickSlot;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZItemActionUI> Btn_Drop;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> Border_ItemTool;
};
