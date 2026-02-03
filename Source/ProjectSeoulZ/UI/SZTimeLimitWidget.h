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
	virtual void Tick(float DeltaSeconds) override;

	// 제한 시간 감소 함수.
	void UpdateTimer(float DeltaTime);

	// Phase1 시작 시 GameMode 호출 API
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StartTimer(float InTotalSeconds, float InWarningSeconds = 60.0f);

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StopTimer();
	
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
	UPROPERTY(VisibleInstanceOnly, Category = "Timer")
	float WarningSeconds = 60.0f;

	UPROPERTY(VisibleInstanceOnly, Category = "Timer")
	bool bFailNotified = false;

	UPROPERTY(VisibleInstanceOnly, Category = "Timer")
	bool bTimerRunning = false;

	UPROPERTY(VisibleInstanceOnly, Category = "Timer")
	FColor NormalColor = FColor::White;

	UPROPERTY(VisibleInstanceOnly, Category = "Timer")
	FColor WarningColor = FColor::Red;
};
