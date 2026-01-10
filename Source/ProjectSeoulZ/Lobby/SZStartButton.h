// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"
#include "SZStartButton.generated.h"

//﻿---------------------------------------------------------------------------------------------------------
// Author		: 안재범
// Date			: 2025-12-18
// Copyright	: CodingDead
// Description	: StartButton 위젯 클래스.
//				  
//----------------------------------------------------------------------------------------------------------

class UButton;
class USZGameInstance;

UCLASS()
class PROJECTSEOULZ_API USZStartButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 버튼 클릭시 실행.
	UFUNCTION()
	void OnStartButtonClicked();

	// 실제 레벨 이동 함수.
	UFUNCTION()
	void OpenSelectedLevel();

protected:
	// 버튼 위젯.
	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;

	// Start Button 클릭 사운드
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* StartButtonSound;

	// 사운드가 재생되고 1초 뒤에 레벨 이동.
	UPROPERTY()
	FTimerHandle LevelOpenTimer;

	// GameInstance
	// USZGameInstance* GI;

};
