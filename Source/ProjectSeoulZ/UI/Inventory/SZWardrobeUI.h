// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SZWardrobeUI.generated.h"

class USZEquipmentSlot;

UCLASS()
class PROJECTSEOULZ_API USZWardrobeUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Wardrobe")
	void SetEquipment();
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZEquipmentSlot> Slot_Helmet;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZEquipmentSlot> Slot_Vest;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZEquipmentSlot> Slot_Gloves;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZEquipmentSlot> Slot_Magazine;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZEquipmentSlot> Slot_Holster;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZEquipmentSlot> Slot_PrimaryGun;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USZEquipmentSlot> Slot_SubGun;

	UPROPERTY(BlueprintReadOnly, Category = "Wardrobe")
	TArray<TObjectPtr<USZEquipmentSlot>> EquipmentSlots;
};
