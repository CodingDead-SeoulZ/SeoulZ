// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/SZAchievementSetBox.h"
#include "Lobby/SZAchievementElement.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/SZGameInstance.h"

#include "Components/ScrollBox.h"

void USZAchievementSetBox::NativeConstruct()
{
	Super::NativeConstruct();
		
	// ScrollBox 델리게이트 바인딩.
	//AchievementScrollBox->FOnAchievementTagSelected.AddDynamic(this, &USZAchievementSetBox::RefreshAchievementElements);
		
	// 초기 화면 표시.
	// AchievementScrollBox 내 WBP_Element들에게 정보 전달.
	PutAchievementElements();
}

void USZAchievementSetBox::PutAchievementElements()
{
	// GameInstance Null 체크.
	USZGameInstance* GI = GetGameInstance<USZGameInstance>();
	if (!GI)
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance is not found"));
		return;
	}
	
	// AchievementScrollBox Null 체크.
	if (!AchievementScrollBox)
	{
		UE_LOG(LogTemp, Error, TEXT("AchievementScrollBox is null"));
		return;
	}
	
	// AchievementElementClass Null 체크.
	if (!AchievementElementClass)
	{
		UE_LOG(LogTemp, Error, TEXT("AchievementElementClass is null"));
		return;
	}
	
	// ScrollBox 초기화.
	AchievementScrollBox->ClearChildren();
	AchievementElementWidgets.Empty();
	
	// 모든 업적 for loop 돌림.
	for (const FAchievementData& Data : GI->AchievementList)
	{
		// 필터 적용.
		if (CurrentTag != ETagType::None && Data.Tag.ToString() != CurrentTagString)
		{
			continue;
		}
		
		// 위젯 생성
		USZAchievementElement* NewElement = CreateWidget<USZAchievementElement>(GetWorld(), AchievementElementClass);
		if (!NewElement)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create AchievementElement"));
			continue;
		}

		// ScrollBox에 추가
		AchievementScrollBox->AddChild(NewElement);

		// 배열에 저장
		AchievementElementWidgets.Add(NewElement);

		// 위젯에 데이터 바인딩
		NewElement->SetAchievementData(Data);
	}

}

void USZAchievementSetBox::OnAchievementTagSelected(ETagType TagType)
{
	// 델리게이트로 선택된 태그를 문자열로 변환
	if (CurrentTag == TagType)
	{
		return; // 같은 버튼 클릭 시 무시
	}

	CurrentTag = TagType;
	switch (CurrentTag)
	{
		case ETagType::None:   CurrentTagString = "None"; break;
		case ETagType::Weapon: CurrentTagString = "Weapon"; break;
		case ETagType::Time:   CurrentTagString = "Time"; break;
		case ETagType::Combat: CurrentTagString = "Combat"; break;
	}
	
	UE_LOG(LogTemp, Log, TEXT("CurrentTag %s"), *CurrentTagString);
	
	PutAchievementElements();
}

//void USZAchievementSetBox::RefreshAchievementElements(ETagType TagType)
//{
//	// 같은 태그 버튼 클릭하면 아무것도 안함.
//	if (SelectedTag == TagType)
//	{
//		UE_LOG(LogTemp, Log, TEXT("AchievementTagChanged | It's on same tag."));
//		return;
//	}
//}
//
//void USZAchievementSetBox::SetSelectedTag(const FString& InSelectedTag)
//{
//	SelectedTag = InSelectedTag;
//	PutAchievementElements();
//}
