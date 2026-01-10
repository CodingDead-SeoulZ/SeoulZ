// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SZTimeLimitWidget.generated.h"

//﻿---------------------------------------------------------------------------------------------------------
// Author		: 안재범
// Date			: 2026-01-09
// Copyright	: CodingDead
// Description	: TimeLimit 위젯 클래스.

//----------------------------------------------------------------------------------------------------------

UCLASS()
class PROJECTSEOULZ_API ASZTimeLimitWidget : public AHUD
{
	GENERATED_BODY()
	
public:
	ASZTimeLimitWidget();
	
	virtual void DrawHUD() override;
	
	// 제한 시간 감소 함수.
	void UpdateTimer(float DeltaTime);
	
public:
	// 제한 시간 (초).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float RemainingTime;
	
	// 제한 시간 텍스트 색상.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	FColor TimerColor;
	
	// 제한 시간 폰트.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	UFont* TimerFont;
	
	// HUD 위치 - 중앙 상단으로 위치 조정.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	FVector2D TimerPosition;
	
	
protected:
	
};
