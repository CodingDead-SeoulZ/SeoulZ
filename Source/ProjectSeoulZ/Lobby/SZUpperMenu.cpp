// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/SZUpperMenu.h"
#include "Components/Button.h"
//#include "Components/Image.h"
//#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/SZGameInstance.h"

void USZUpperMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	// GameInstance 불러오기.
	if (!GI)
	{
		GI = GetWorld()->GetGameInstance<USZGameInstance>();
	}

	// GameInstance* 변수 GI 유효성 검사.
	if (!GI || GI->MapList.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("GameInstance object is not created properly."));
		return;
	}

	// Button 바인딩.
	PlayButton			->OnClicked.AddDynamic(this, &USZUpperMenu::OnPlayButtonClicked);
	ShopButton			->OnClicked.AddDynamic(this, &USZUpperMenu::OnShopButtonClicked);
	AchievementButton	->OnClicked.AddDynamic(this, &USZUpperMenu::OnAchievementButtonClicked);
	GarageButton		->OnClicked.AddDynamic(this, &USZUpperMenu::OnGarageButtonClicked);

}

void USZUpperMenu::NotifyMenuSelected(ELobbyMenuType MenuType)
{
	// 클릭되었을 때 사운드 재생.
	if (MenuButtonSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), MenuButtonSound);
	}
	
	// 델리게이트 broadcast
	OnLobbyMenuSelected.Broadcast(MenuType);
}

void USZUpperMenu::OnPlayButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("PlayButton is clicked"));

	// 델리게이트 함수 호출.
	NotifyMenuSelected(ELobbyMenuType::Play);
}

void USZUpperMenu::OnShopButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("ShopButton is clicked"));

	// 델리게이트 함수 호출.
	NotifyMenuSelected(ELobbyMenuType::Shop);
}

void USZUpperMenu::OnAchievementButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("AchievementButton is clicked"));

	// 델리게이트 함수 호출.
	NotifyMenuSelected(ELobbyMenuType::Achievement);
}

void USZUpperMenu::OnGarageButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("GarageButton is clicked"));

	// 델리게이트 함수 호출.
	NotifyMenuSelected(ELobbyMenuType::Garage);
}
