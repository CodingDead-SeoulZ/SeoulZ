// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/SZStartButton.h"
#include "SZStartButton.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/SZGameInstance.h"

void USZStartButton::NativeConstruct()
{
	Super::NativeConstruct();

	// GameInstance 불러오기.
	if (!GI)
	{
		GI = GetWorld()->GetGameInstance<USZGameInstance>();
	}

	// Start Button 바인딩.
	StartButton->OnClicked.AddDynamic(this, &USZStartButton::OnStartButtonClicked);
	UE_LOG(LogTemp, Log, TEXT("StartButton binded"));
}

void USZStartButton::OnStartButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("StartButton is clicked"));

	// 클릭 사운드 재생.
	if (StartButtonSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), StartButtonSound);
	}

	// 1초 뒤에 레벨 이동 타이머 재생.
	GetWorld()->GetTimerManager().SetTimer(
		LevelOpenTimer,
		this,
		&USZStartButton::OpenSelectedLevel,
		1.0f,
		false
	);
}

void USZStartButton::OpenSelectedLevel()
{
	if (!GI || !(GI->MapList.IsValidIndex(GI->SelectedMapIndex)))
	{
		return;
	}

	// 인게임 레벨 이동.
	UGameplayStatics::OpenLevel(
		GetWorld(),
		GI->MapList[GI->SelectedMapIndex].LevelName
	);
}
