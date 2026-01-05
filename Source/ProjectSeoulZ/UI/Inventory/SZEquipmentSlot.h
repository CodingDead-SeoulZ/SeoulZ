// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/SZItemDefine.h"
#include "SZEquipmentSlot.generated.h"

class UButton;
class UBorder;
class UImage;
class USZCharacterEquipmentComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotSelected,EEquipmentSlotType, SlotType);


UCLASS()
class PROJECTSEOULZ_API USZEquipmentSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "EquipmentSlot")
	void RefreshEquipmentSlot();

protected:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UFUNCTION(BlueprintCallable, Category = "EquipmentSlot")
	void SetEmptySlot();

	UFUNCTION(BlueprintCallable, Category = "EquipmentSlot")
	void SetFullSlot(UTexture2D* InTexture);

	// UFUNCTION()
	// void HandleSlotClicked();

	UFUNCTION(BlueprintCallable, Category = "EquipmentSlot")
	bool CheckItemSlot(FName InItemID, EEquipmentSlotType InEquipmentSlot) const;

	UFUNCTION(BlueprintCallable, Category = "EquipmentSlot")
	void Refresh();

	UFUNCTION(BlueprintCallable, Category = "EquipmentSlot")
	void EquipItem();

public:
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
	TObjectPtr<USZCharacterEquipmentComponent> SZCharacterEquipment;

	UPROPERTY(BlueprintAssignable, Category = "EquipmentSlot")
	FOnSlotSelected OnSlotSelected;

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
