// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/SZAchievementBase.h"

void USZAchievementBase::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!AchievementSection)
	{
		UE_LOG(LogTemp, Error, TEXT("AchievementSection is null"));
		return;
	}

	if (!AchievementSetBox)
	{
		UE_LOG(LogTemp, Error, TEXT("AchievementSetBox is null"));
		return;
	}

	// Section의 버튼 클릭 델리게이트를 SetBox 함수에 연결
	AchievementSection->OnAchievementTagSelected.AddDynamic(AchievementSetBox, &USZAchievementSetBox::OnAchievementTagSelected);
	
}
