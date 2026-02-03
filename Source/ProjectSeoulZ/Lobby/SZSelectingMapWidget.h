// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SZSelectingMapWidget.generated.h"

//﻿---------------------------------------------------------------------------------------------------------
// Author		: 안재범
// Date			: 2025-12-18
// Copyright	: CodingDead
// Description	: ModeSelectBox 위젯 클래스.
//				  
//----------------------------------------------------------------------------------------------------------

class UButton;
class UImage;
class UTextBlock;
class USZGameInstance;

UCLASS()
class PROJECTSEOULZ_API USZSelectingMapWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	virtual void NativeConstruct() override;
	
	// 왼쪽 버튼 동작.
	UFUNCTION()
	void OnLeftButtonClicked();

	// 오른쪽 버튼 동작.
	UFUNCTION()
	void OnRightButtonClicked();

	// 미리보기 이미지.
	void UpdatePreview();

// 버튼.
protected:
	// 왼쪽 버튼.
	UPROPERTY(meta = (BindWidget))
	UButton* LeftButton;

	// 오른쪽 버튼.
	UPROPERTY(meta = (BindWidget))
	UButton* RightButton;

	// 미리보기 이미지.
	UPROPERTY(meta = (BindWidget))
	UImage* PreviewImage;

	// @Todo. 맵 이름.
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MapNameText;

	// Game Instance
	USZGameInstance* GI;
	
};
