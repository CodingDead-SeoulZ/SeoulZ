// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/SZLobbyRoot.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void USZLobbyRoot::NativeConstruct()
{
	Super::NativeConstruct();

	// 처음에는 시작 메뉴가 와야함.
	CurrentMenu = ELobbyMenuType::Play;

	// UpperMenu 델리게이트 바인딩.
	UpperMenu->OnLobbyMenuSelected.AddDynamic(this, &USZLobbyRoot::HandleMenuChanged);

	// 초기 화면 표시.
	ShowPlayMenu();
}

void USZLobbyRoot::HandleMenuChanged(ELobbyMenuType MenuType)
{
	// 같은 메뉴 클릭하면 아무것도 안함.
	if (CurrentMenu == MenuType)
	{
		UE_LOG(LogTemp, Log, TEXT("HandleMenuChanged | It's on same menu."));
		return;
	}

	// 다른 메뉴 클릭시
	UE_LOG(LogTemp, Log, TEXT("HandleMenuChanged | Currnet: %d | Incoming: %d"), 
		(int32)CurrentMenu, 
		(int32)MenuType
	);
	CurrentMenu = MenuType;
	
	// 메뉴 위젯 show
	switch (MenuType)
	{
	case ELobbyMenuType::Play:
		ShowPlayMenu();
		break;
	case ELobbyMenuType::Shop:
		ShowShopMenu();
		break;
	case ELobbyMenuType::Achievement:
		ShowAchievementMenu();
		break;
	case ELobbyMenuType::Garage:
		ShowGarageMenu();
		break;
	default:
		break;
	}
}

void USZLobbyRoot::ShowPlayMenu()
{
	UE_LOG(LogTemp, Log, TEXT("Show PlayMenu"));
	
	// Widget Swithcer로 메뉴 이동.
	if (ContentSwitcher)
	{
		ContentSwitcher->SetActiveWidgetIndex(0);
	}
}

void USZLobbyRoot::ShowShopMenu()
{
	UE_LOG(LogTemp, Log, TEXT("Show ShopMenu"));
	
	// Widget Swithcer로 메뉴 이동.
	if (ContentSwitcher)
	{
		ContentSwitcher->SetActiveWidgetIndex(1);
	}
}

void USZLobbyRoot::ShowAchievementMenu()
{
	UE_LOG(LogTemp, Log, TEXT("Show AchievementMenu"));
	
	// Widget Swithcer로 메뉴 이동.
	if (ContentSwitcher)
	{
		ContentSwitcher->SetActiveWidgetIndex(2);
	}
}

void USZLobbyRoot::ShowGarageMenu()
{
	UE_LOG(LogTemp, Log, TEXT("Show GarageMenu"));
	
	// Widget Swithcer로 메뉴 이동.
	if (ContentSwitcher)
	{
		ContentSwitcher->SetActiveWidgetIndex(3);
	}
}
