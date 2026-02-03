// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SZItemSlider.generated.h"

class USZInventoryBaseComponent;
class UBorder;
class UTextBlock;
class UButton;
class USlider;

UCLASS()
class PROJECTSEOULZ_API USZItemSlider : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	int32 GetItemStack() const;
	void SetSliderValue();
	void SetSlotStack();

	UFUNCTION()
	void OnSliderValueChanged(float Value);

	UFUNCTION()
	void OnClickedCancel();

	UFUNCTION()
	void OnClickedDrop();

	UFUNCTION()
	void SetItemSliderPosition();

public:
	UPROPERTY(BlueprintReadOnly, Transient, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<USZInventoryBaseComponent> SZInventoryBase;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (ExposeOnSpawn = "true"))
	int32 Index = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (ExposeOnSpawn = "true"))
	int32 ItemStack = 0;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> Border_Slider;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_ItemStack;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Cancel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Drop;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USlider> Slider_ItemStack;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_Cancel;
};
