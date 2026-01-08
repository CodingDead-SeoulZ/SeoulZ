// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SZGameInstance.generated.h"

//﻿---------------------------------------------------------------------------------------------------------
// Author		: 안재범
// Date			: 2025-12-15
// Copyright	: CodingDead
// Description	: GameInstance 클래스.
//				  맵 목록 정보만 들어있음.
//----------------------------------------------------------------------------------------------------------

// 맵 이름과 미리보기 이미지를 구조체에 담음.
USTRUCT(BlueprintType)
struct FMapData
{
	GENERATED_BODY()

public:
	// 맵 이름.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelName;

	// 맵 미리보기 이미지.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* PreviewImage;
};

UCLASS()
class PROJECTSEOULZ_API USZGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	//void BeginLoadingScreen(const FString& LevelName);
	//void EndLoadingScreen(const FString& LevelName);

public:
	// 맵 + 이미지 목록.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMapData> MapList;

	// 맵 인덱스 -> 모듈러 써야해서.
	UPROPERTY(BlueprintReadWrite)
	int32 SelectedMapIndex = 0;

};
