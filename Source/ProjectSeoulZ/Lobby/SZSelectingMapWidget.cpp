// Fill out your copyright notice in the Description page of Project Settings.

#include "Lobby/SZSelectingMapWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/SZGameInstance.h"

void USZSelectingMapWidget::NativeConstruct()
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

	// LeftButton 바인딩.
	LeftButton->OnClicked.AddDynamic(this, &USZSelectingMapWidget::OnLeftButtonClicked);
	// RightButton 바인딩.
	RightButton->OnClicked.AddDynamic(this, &USZSelectingMapWidget::OnRightButtonClicked);

	// 맵 프리뷰 이미지 갱신.
	UpdatePreview();
}

void USZSelectingMapWidget::OnLeftButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("LeftButton is clicked"));

	// LeftButton Null 체크.
	if (!GI)
	{
		UE_LOG(LogTemp, Log, TEXT("LeftButton is None."));
		return;
	}

	// 맵 인덱스 변경.
	GI->SelectedMapIndex = (GI->SelectedMapIndex - 1 + GI->MapList.Num()) % GI->MapList.Num();
	
	// 맵 프리뷰 이미지 갱신.
	UpdatePreview();
}

void USZSelectingMapWidget::OnRightButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("RightButton is clicked"));

	// RightButton Null 체크.
	if (!GI)
	{
		UE_LOG(LogTemp, Log, TEXT("RightButton is None."));
		return;
	}

	// 맵 인덱스 변경.
	GI->SelectedMapIndex = (GI->SelectedMapIndex + 1) % GI->MapList.Num();
	
	// 맵 프리뷰 이미지 갱신.
	UpdatePreview();
}

void USZSelectingMapWidget::UpdatePreview()
{
	UE_LOG(LogTemp, Log, TEXT("Index of SelectedMap: %d"), GI->SelectedMapIndex);

	// 맵 프리뷰 이미지 갱신.
	if (PreviewImage && GI->MapList.IsValidIndex(GI->SelectedMapIndex))
	{
		PreviewImage->SetBrushFromTexture(GI->MapList[GI->SelectedMapIndex].PreviewImage);
	}
}