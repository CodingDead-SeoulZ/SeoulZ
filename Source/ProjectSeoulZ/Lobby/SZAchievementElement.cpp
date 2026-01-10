// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/SZAchievementElement.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USZAchievementElement::NativeConstruct()
{
	Super::NativeConstruct();
}

void USZAchievementElement::SetAchievementData(const FAchievementData& InAchievementData)
{
	// 데이터 가져오기.
	
	CurrentData = InAchievementData;
	
	AchievementIcon->SetBrushFromTexture(CurrentData.Icon);
	AchievementName->SetText(CurrentData.Name);
	AchievementDescription->SetText(CurrentData.Description);
	AchievementCompletion->SetPercent(CurrentData.Progress);
	AchievementLockOverlay->SetVisibility(CurrentData.bUnlocked ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

