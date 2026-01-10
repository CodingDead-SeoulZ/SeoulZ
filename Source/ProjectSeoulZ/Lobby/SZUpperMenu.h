// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundBase.h"
#include "SZUpperMenu.generated.h"

//﻿---------------------------------------------------------------------------------------------------------
// Author		: 안재범
// Date			: 2026-01-06
// Copyright	: CodingDead
// Description	: 로비 상단 메뉴 위젯 클래스
//				  
//----------------------------------------------------------------------------------------------------------

class UButton;
class USZGameInstance;

// 메뉴 항목들.
UENUM(BlueprintType)
enum class ELobbyMenuType : uint8
{
	Play,
	Shop,
	Achievement,
	Garage
};

// 상단 메뉴 선택 델리게이트 선언.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyMenuSelected, ELobbyMenuType, MenuType);

UCLASS()
class PROJECTSEOULZ_API USZUpperMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	// 메뉴 선택 이벤트.
	UPROPERTY(BlueprintAssignable, Category="Lobby|Menu")
	FOnLobbyMenuSelected OnLobbyMenuSelected;

public:
	// 버튼 클릭 공동 처리.
	virtual void NativeConstruct() override;

	void NotifyMenuSelected(ELobbyMenuType MenuType);

	// 게임 시작 메뉴 이동 버튼 바인딩 함수.
	UFUNCTION()
	void OnPlayButtonClicked();

	// 상점 메뉴 이동 버튼.
	UFUNCTION()
	void OnShopButtonClicked();

	// 업적 메뉴 이동 버튼.
	UFUNCTION()
	void OnAchievementButtonClicked();

	// 창고 메뉴 이동 버튼.
	UFUNCTION()
	void OnGarageButtonClicked();

protected:

	// 게임 시작 메뉴 버튼.
	UPROPERTY(meta = (BindWidget))
	UButton* PlayButton;

	// 상점 메뉴 버튼.
	UPROPERTY(meta = (BindWidget))
	UButton* ShopButton;

	// 업적 메뉴 버튼.
	UPROPERTY(meta = (BindWidget))
	UButton* AchievementButton;

	// 창고 메뉴 버튼.
	UPROPERTY(meta = (BindWidget))
	UButton* GarageButton;

	// Button 클릭 사운드
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* MenuButtonSound;

	// Game Instance
	USZGameInstance* GI;

	
};
