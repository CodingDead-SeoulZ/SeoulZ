// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/SZGameInstance.h"
#include "SZAchievementElement.generated.h"

//﻿---------------------------------------------------------------------------------------------------------
// Author		: 안재범
// Date			: 2026-01-08
// Copyright	: CodingDead
// Description	: 업적 위젯 클래스.
//				  
//----------------------------------------------------------------------------------------------------------

class UTextBlock;
class UImage;
class UProgressBar;

UCLASS()
class PROJECTSEOULZ_API USZAchievementElement : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	// 업적 데이터.
	UFUNCTION(BlueprintCallable)
	void SetAchievementData(const FAchievementData& InAchievementData);

protected:
	// 현재 표시 중인 위젯.
	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite, Category = "Achievement")
	FAchievementData CurrentData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* AchievementIcon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AchievementName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* AchievementDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* AchievementCompletion;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* AchievementLockOverlay;
	
//protected:
//	// 현재 데이터
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	FName AchievementId;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	FText Name;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	FText Description;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	UTexture2D* Icon;
//	
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	FText Tag;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	bool bUnlocked;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	float Progress; // 0.0 ~ 1.0
	
};
