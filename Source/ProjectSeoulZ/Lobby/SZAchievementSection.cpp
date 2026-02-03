// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/SZAchievementSection.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "SZAchievementSetBox.h"

void USZAchievementSection::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 버튼 바인딩
	ButtonAll	->OnClicked.AddDynamic(this, &USZAchievementSection::OnAllClicked);
	ButtonWeapon->OnClicked.AddDynamic(this, &USZAchievementSection::OnWeaponClicked);
	ButtonTime	->OnClicked.AddDynamic(this, &USZAchievementSection::OnTimeClicked);
	ButtonCombat->OnClicked.AddDynamic(this, &USZAchievementSection::OnCombatClicked);
	
}

void USZAchievementSection::NotifyTagSelected(ETagType TagType)
{
	// 클릭되었을 때 사운드 재생.
	if (TagButtonSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), TagButtonSound);
	}
	
	// 델리게이트 broadcast
	OnAchievementTagSelected.Broadcast(TagType);
}

void USZAchievementSection::OnAllClicked()
{
	UE_LOG(LogTemp, Log, TEXT("ButtonAll is clicked"));
	
	// 델리게이트 함수 호출.
	NotifyTagSelected(ETagType::None);
}

void USZAchievementSection::OnWeaponClicked()
{
	UE_LOG(LogTemp, Log, TEXT("ButtonWeapon is clicked"));
	
	// 델리게이트 함수 호출.
	NotifyTagSelected(ETagType::Weapon);
}

void USZAchievementSection::OnTimeClicked()
{
	UE_LOG(LogTemp, Log, TEXT("ButtonTime is clicked"));
	
	// 델리게이트 함수 호출.
	NotifyTagSelected(ETagType::Time);
}

void USZAchievementSection::OnCombatClicked()
{
	UE_LOG(LogTemp, Log, TEXT("ButtonCombat is clicked"));
	
	// 델리게이트 함수 호출.
	NotifyTagSelected(ETagType::Combat);
}
