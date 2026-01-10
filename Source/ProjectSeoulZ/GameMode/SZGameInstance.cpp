// Fill out your copyright notice in the Description page of Project Settings.

#include "SZGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Lobby/SZAchievementId.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "UObject/UObjectGlobals.h"

void USZGameInstance::Init()
{
	Super::Init();

	// 맵.
	InitializeMapList();
	// 업적.
	InitializeAchievement();
}

void USZGameInstance::InitializeMapList()
{
	// 초기화.
	MapList.Empty();

	// 첫 번째 맵.
	FMapData Map1;
	Map1.LevelName = FName("TestGameLevel1");
	Map1.PreviewImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Test/AHNJB/Preview_Level1.Preview_Level1"));
	MapList.Add(Map1);

	// 두 번째 맵.
	FMapData Map2;
	Map2.LevelName = FName("TestGameLevel2");
	Map2.PreviewImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Test/AHNJB/Preview_Level2.Preview_Level2"));
	MapList.Add(Map2);

	// 세 번째 맵.
	FMapData Map3;
	Map3.LevelName = FName("Map_Post-Apocalyptic_DayLight");
	Map3.PreviewImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Map/PA_UrbanCity/Preview_Phase1.Preview_Phase1"));
	MapList.Add(Map3);
	
	// MapList 체크용 UE_LOG
	for (int32 Index = 0; Index < MapList.Num(); ++Index)
	{
		const FMapData& Data = MapList[Index];
		UE_LOG(LogTemp, Log, TEXT("[Map][%d] MapName = %s"), Index, *Data.LevelName.ToString());
	}
	
}

//void USZGameInstance::LoadSelectedLevelAsync()
//{
//	// 비동기 레벨 로드 시작.
//	
//	// 맵 리스트 유효성 검사.
//	if (!MapList.IsValidIndex(SelectedMapIndex))
//	{
//		return;
//	}
//	
//	// 로딩화면 표시
//	if (LoadingScreenClass && !LoadingScreenWidget)
//	{
//		LoadingScreenWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenClass);
//		if (LoadingScreenWidget)
//		{
//			// 화면에 LoadingScreenWidget 표시.
//			LoadingScreenWidget->AddToViewport();
//		}
//	}
//	
//	// 패키지 경로.
//	FString LevelNameStr = MapList[SelectedMapIndex].LevelName.ToString();
//	FString PackagePath = FString::Printf(TEXT("/Game/Map/PA_UrbanCity/Maps/%s"), *LevelNameStr);
//	FName PackageFName(*PackagePath);
//	
//	// Async Load Package (비동기 로드 시작)
//	LoadPackageAsync(PackageFName, FLoadPackageAsyncDelegate::CreateUObject(this, &USZGameInstance::OnLevelPackageLoaded));
//	
//}

//void USZGameInstance::OnLevelPackageLoaded(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
//{
//	// LoadedPackage Null 체크.
//	if (!LoadedPackage)
//	{
//		UE_LOG(LogTemp, Error, TEXT("Async Load failed"));
//		if (LoadingScreenWidget)
//		{
//			LoadingScreenWidget->RemoveFromParent();
//			LoadingScreenWidget = nullptr;
//		}
//		return;
//	}
//
//	// 로딩 완료 → OpenLevel
//	if (MapList.IsValidIndex(SelectedMapIndex))
//	{
//		UGameplayStatics::OpenLevel(GetWorld(), MapList[SelectedMapIndex].LevelName);
//	}
//}

void USZGameInstance::InitializeAchievement()
{
	// 초기화.
	AchievementList.Empty();
	
	// 업적 1 - 페이즈 1을 권총으로만 클리어.
	FAchievementData Data1;
	Data1.AchievementId = ACH_PISTOL_ONLY_PHASE1;
	Data1.Name = FText::FromString("Gun Specialist");
	Data1.Description = FText::FromString("Clear Phase 1 using only a pistol");
	Data1.Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Test/AHNJB/Preview_Level1.Preview_Level1"));
	Data1.Tag = FText::FromString("Weapon");
	Data1.bUnlocked = false;
	Data1.Progress = 0.0f;
	AchievementList.Add(Data1);
	
	// 업적 2 - 페이즈를 클리어.
	FAchievementData Data2;
	Data2.AchievementId = ACH_TIME_LEFT_CLEAR;
	Data2.Name = FText::FromString("Against the Clock");
	Data2.Description = FText::FromString("Clear a phase with time remaining");
	Data2.Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Test/AHNJB/Preview_Level1.Preview_Level1"));
	Data2.Tag = FText::FromString("Time");
	Data2.bUnlocked = false;
	Data2.Progress = 0.0f;
	AchievementList.Add(Data2);
	
	// 업적 3 - 페이즈 1을 4분 이내, 페이즈 2는 6분 이내로 클리어.
	FAchievementData Data3;
	Data3.AchievementId = ACH_SPEED_CLEAR;
	Data3.Name = FText::FromString("Speed Runner");
	Data3.Description = FText::FromString("Clear Phase 1 within 4 minutes and Phase 2 within 6 minutes");
	Data3.Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Test/AHNJB/Preview_Level1.Preview_Level1"));
	Data3.Tag = FText::FromString("Time");
	Data3.bUnlocked = false;
	Data3.Progress = 0.0f;
	AchievementList.Add(Data3);
	
	// 테스트용 업적 1 - 첫 킬 달성.
	FAchievementData Data4;
	Data4.AchievementId = ACH_FIRST_KILL;
	Data4.Name = FText::FromString("First Blood");
	Data4.Description = FText::FromString("Defeat your first enemy");
	Data4.Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Test/AHNJB/Preview_Level1.Preview_Level1"));
	Data4.Tag = FText::FromString("Combat");
	Data4.bUnlocked = false;
	Data4.Progress = 0.0f;
	AchievementList.Add(Data4);
	
	// 테스트용 업적 2 - 페이즈 1 클리어.
	FAchievementData Data5;
	Data5.AchievementId = ACH_CLEAR_PHASE1;
	Data5.Name = FText::FromString("Phase 1 Clear");
	Data5.Description = FText::FromString("Clear Phase 1");
	Data5.Icon = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Test/AHNJB/Preview_Level1.Preview_Level1"));
	Data5.Tag = FText::FromString("Combat");
	Data5.bUnlocked = false;
	Data5.Progress = 0.0f;
	AchievementList.Add(Data5);
	
	// AchievementList 체크용 UE_LOG
	for (int32 Index = 0; Index < AchievementList.Num(); ++Index)
	{
		const FAchievementData& Data = AchievementList[Index];

		UE_LOG(LogTemp, Log, 
			TEXT("[Achievement][%d] Id=%s | Name=%s | Description=%s | Icon=%s | Tag=%s | Unlocked=%s"),
			Index,
			*Data.AchievementId.ToString(),
			*Data.Name.ToString(),
			*Data.Description.ToString(),
			Data.Icon ? TEXT("true") : TEXT("false"),
			*Data.Tag.ToString(),
			Data.bUnlocked ? TEXT("true") : TEXT("false")
		);
	}
}

//void USZGameInstance::UnlockAchievement(const FString& AchievementId)
//{
//	for (FAchievementUIData& Data : AchievementList)
//	{
//		if (Data.AchievementId == AchievementId)
//		{
//			//Data.bUnlocked = true;
//			Data.Progress = 1.0f;
//			break;
//		}
//	}
//}
