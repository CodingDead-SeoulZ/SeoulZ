// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SZAchievementSection.h"
#include "SZAchievementSetBox.h"
#include "SZAchievementBase.generated.h"

//﻿---------------------------------------------------------------------------------------------------------
// Author		: 안재범
// Date			: 2026-01-09
// Copyright	: CodingDead
// Description	: 로비 화면 위젯 클래스. 여기서 메뉴 이동함.
//				  
//----------------------------------------------------------------------------------------------------------

class USZAchievementSection;
class USZAchievementSetBox;

UCLASS()
class PROJECTSEOULZ_API USZAchievementBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
protected:
	// 좌측 Section (태그 버튼 영역)
	UPROPERTY(meta = (BindWidget))
	USZAchievementSection* AchievementSection;

	// 우측 SetBox (ScrollBox 영역)
	UPROPERTY(meta = (BindWidget))
	USZAchievementSetBox* AchievementSetBox;
};
