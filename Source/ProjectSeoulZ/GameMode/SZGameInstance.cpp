// Fill out your copyright notice in the Description page of Project Settings.


#include "SZGameInstance.h"

//USZGameInstance::USZGameInstance()
//{
//	// @Todo: 처음은 로비 레벨을 불러와야함.
//
//	// @Todo: 맵 배열 생성.
//
//}

void USZGameInstance::Init()
{
	Super::Init();

	//
	MapList.Empty();

	//
	FMapData Map1;
	Map1.LevelName = FName("TestGameLevel1");
	Map1.PreviewImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Test/AHNJB/Preview_Level1.Preview_Level1"));

	//
	FMapData Map2;
	Map2.LevelName = FName("TestGameLevel2");
	Map2.PreviewImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Test/AHNJB/Preview_Level2.Preview_Level2"));

	//
	FMapData Map3;
	Map3.LevelName = FName("Map_Post-Apocalyptic_DayLight");
	Map3.PreviewImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Map/PA_UrbanCity/Preview_Phase1.Preview_Phase1"));

	//
	MapList.Add(Map1);
	MapList.Add(Map2);
	MapList.Add(Map3);
}
