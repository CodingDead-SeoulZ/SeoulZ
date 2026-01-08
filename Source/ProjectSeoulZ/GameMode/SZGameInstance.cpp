// Fill out your copyright notice in the Description page of Project Settings.

#include "SZGameInstance.h"

void USZGameInstance::Init()
{
	Super::Init();

	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// 맵 리스트 초기화.
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

}
