// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Engine/StreamableManager.h"
#include "UObject/Package.h"
#include "UObject/UObjectGlobals.h"
#include "SZGameInstance.generated.h"

//﻿---------------------------------------------------------------------------------------------------------
// Author		: 안재범
// Date			: 2025-12-15
// Copyright	: CodingDead
// Description	: GameInstance 클래스.
//				  맵 목록 정보 들어있음.
//				  업적 목록 정보 들어있음.
//				  비동기 로딩 구현.
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

// 업적 데이터 구조체.
USTRUCT(BlueprintType)
struct FAchievementData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AchievementId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUnlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Progress; // 0.0 ~ 1.0
};

// Phase Enum
UENUM(BlueprintType)
enum class ESZPhase : uint8
{
	Phase1,
	Phase2
};

UCLASS()
class PROJECTSEOULZ_API USZGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
public:
	// 맵 초기화.
	UFUNCTION(BlueprintCallable, Category = "Map")
	void InitializeMapList();
	
	// 비동기 레벨 로드 시작
	//UFUNCTION()
	//void LoadSelectedLevelAsync();

	// 레벨 패키지 로드 완료 콜백.
	// LoadPackage == nullptr이면 실패, 아니면 성공.
	//UFUNCTION()
	//void OnLevelPackageLoaded(const FName& PackageName, UPackage* LoadedPackage, TEnumAsByte<EAsyncLoadingResult::Type> Result);	
	
	//void BeginLoadingScreen(const FString& LevelName);
	//void EndLoadingScreen(const FString& LevelName);
	
public:
	// 업적 초기화.
	UFUNCTION(BlueprintCallable, Category = "Achievement")
	void InitializeAchievement();
	
	// 업적 해금.
	//UFUNCTION(BlueprintCallable, Category = "Achievement")
	//void UnlockAchievement(const FString& AchievementId);
	
	// Phase2 전환
	UFUNCTION(BlueprintCallable, Category = "Phase")
	void SetPhase2()
	{
		CurrentPhase = ESZPhase::Phase2;
		CurrentPhaseLimitSeconds = Phase2LimitSeconds;
	}

public:
	// 맵 + 이미지 목록.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMapData> MapList;

	// 맵 인덱스 -> 모듈러 써야해서.
	UPROPERTY(BlueprintReadWrite)
	int32 SelectedMapIndex = 0;
	
	// 로딩 화면 위젯 블루프린트 클래스
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<class UUserWidget> LoadingScreenClass;

	// 로딩 화면 위젯 인스턴스
	//UPROPERTY()
	//UUserWidget* LoadingScreenWidget;
	
	// phase1 제한시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission|Timer")
	float Phase1LimitSeconds = 304.0f;

	// phase2 제한시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission|Timer")
	float Phase2LimitSeconds = 420.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission|Timer")
	float WarningSeconds = 60.0f;

	// 현재 페이즈 상태
	UPROPERTY(BlueprintReadWrite, Category = "Phase")
	ESZPhase CurrentPhase = ESZPhase::Phase1;

	// 현재 제한시간(실제로 GameMode/HUD가 이 값을 보고 시작)
	UPROPERTY(BlueprintReadWrite, Category = "Phase")
	float CurrentPhaseLimitSeconds = 300.f;

	// 페이즈 초기화
	UFUNCTION(BlueprintCallable, Category = "Phase")
	void ResetPhaseState()
	{
		CurrentPhase = ESZPhase::Phase1;
	}

public:
	// 업적 목록.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Achievement")
	TArray<FAchievementData> AchievementList;
};
