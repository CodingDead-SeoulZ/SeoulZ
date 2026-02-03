// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SZItemCategory.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2025-12-26
// Copyright    : 
//
// Description  : Item Category 위젯
//              위젯의 텍스트 및 버튼 배경색 제어
//                 
//----------------------------------------------------------------------------------------------------------

class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFilterClicked, bool, bIsOn, int32, FilterId);

UCLASS()
class PROJECTSEOULZ_API USZItemCategory : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleButtonClicked();

public:
	UPROPERTY(BlueprintAssignable)
	FOnFilterClicked OnFilterClicked;

public:
	// 에디터에서 각 카테고리마다 바꿀 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Category")
	FText FilterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Category", meta = (Bitmask, BitmaskEnum = "EItemCategory"))
	int32 FilterID = 0;

	// 바인드 위젯
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Button_Filter;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_FilterName;

	// 버튼 색상
	UPROPERTY(EditDefaultsOnly, Category = "Filter")
	FLinearColor Button_On = FLinearColor(0.698f, 0.698f, 0.698f, 1.f);

	UPROPERTY(EditDefaultsOnly, Category = "Filter")
	FLinearColor Button_Off = FLinearColor(0.042311f, 0.042311f, 0.042311f, 1.f);

private:
	bool bIsOn;
};
