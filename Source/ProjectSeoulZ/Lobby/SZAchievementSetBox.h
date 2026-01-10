// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SZAchievementElement.h"
#include "SZAchievementSection.h"
#include "SZAchievementSetBox.generated.h"

//﻿---------------------------------------------------------------------------------------------------------
// Author		: 안재범
// Date			: 2026-01-08
// Copyright	: CodingDead
// Description	: 업적들이 들어있는 위젯 클래스.
//				  
//----------------------------------------------------------------------------------------------------------

class UScrollBox;
class USZAchievementElement;

UCLASS()
class PROJECTSEOULZ_API USZAchievementSetBox : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	// ScrollBox안에 Element들을 넣어야 함.
	void PutAchievementElements();
		
	// 태그 설정용 함수 - Section의 델리게이트에서 호출.
	UFUNCTION()
	void OnAchievementTagSelected(ETagType TagType);

protected:
	// 업적들을 담을 Scroll Box.
	UPROPERTY(meta = (BindWidget))
	UScrollBox* AchievementScrollBox;
	
	// Achievement Element BP 클래스.
	UPROPERTY(EditAnywhere, Category = "Achievement")
	TSubclassOf<USZAchievementElement> AchievementElementClass;
	
	// 생성된 위젯 인스턴스 배열
	UPROPERTY()
	TArray<USZAchievementElement*> AchievementElementWidgets;
	
	// 현재 활성화된 태그.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Achievement")
	ETagType CurrentTag = ETagType::None;	
	
	FString CurrentTagString;
};
