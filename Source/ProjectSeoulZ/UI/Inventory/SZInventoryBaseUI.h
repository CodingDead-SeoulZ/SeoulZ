// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/Components/SZInventoryBaseComponent.h"
#include "SZInventoryBaseUI.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-12-26
// Copyright    : 
//
// Description  : Inventory Base 사용자 위젯
//               슬롯을 만들고, 아이템을 표시하고, 필터를 적용하는 공통 UI 부모 클래스 로직
//               - InventoryUI는 Inventory 컴포넌트를, QuickSlotUI은 Quick 컴포넌트를 사용.
//               - 컴포넌트는 Getter 함수로 구분.
//                 
//----------------------------------------------------------------------------------------------------------

class USZInventorySlot;
class UWrapBox;

UCLASS()
class PROJECTSEOULZ_API USZInventoryBaseUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	void RefreshInventory(USZInventoryBaseComponent* InSZInventory);

	UFUNCTION(BlueprintCallable)
	void CreateInventorySlots();

	UFUNCTION(BlueprintCallable)
	bool FilterItem(const FItemSlot& InSlot) const;

	UFUNCTION(BlueprintCallable)
	bool FilterEmpty(const FItemSlot& InSlot) const;

	UFUNCTION()
	void OnFilterClicked(bool bIsOn, int32 FilterId);

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USZInventorySlot> InventorySlotClass;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> WrapBox_Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter")
	bool bIsFilterActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Filter", meta = (Bitmask, BitmaskEnum = "EItemCategory"))
	int32 CurrentFilter = 0;

protected:
	TObjectPtr<USZInventoryBaseComponent> SZInventory;
};
