// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SZUpperMenu.h"
#include "SZLobbyRoot.generated.h"

//﻿---------------------------------------------------------------------------------------------------------
// Author		: 안재범
// Date			: 2026-01-07
// Copyright	: CodingDead
// Description	: 로비 화면 위젯 클래스. 여기서 메뉴 이동함.
//				  
//----------------------------------------------------------------------------------------------------------

class UWidgetSwitcher;

UCLASS()
class PROJECTSEOULZ_API USZLobbyRoot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	// 상단 메뉴에서 메뉴 선택시 호출
	UFUNCTION()
	void HandleMenuChanged(ELobbyMenuType MenuType);

	// 메뉴별 화면 전환 함수.
	void ShowPlayMenu();
	void ShowShopMenu();
	void ShowAchievementMenu();
	void ShowGarageMenu();

protected:

	// 현재 활성화된 메뉴.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lobby|State")
	ELobbyMenuType CurrentMenu;

	// 상단 메뉴 위젯.
	UPROPERTY(meta = (BindWidget))
	USZUpperMenu* UpperMenu;

	// WidgetSwitcher/
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ContentSwitcher;

	// 메뉴 위젯 클래스들.
	UPROPERTY(EditDefaultsOnly, Category = "Lobby|Widget")
	TSubclassOf<UUserWidget> PlayMenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Lobby|Widget")
	TSubclassOf<UUserWidget> ShopMenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Lobby|Widget")
	TSubclassOf<UUserWidget> AchievementMenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Lobby|Widget")
	TSubclassOf<UUserWidget> GarageMenuWidgetClass;

	
};
