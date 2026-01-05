// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SZPoolManager.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 이혜성
// Date         : 2025-12-00
// Copyright    : CodingDead
//
// Description  : 오브젝트 풀링을 위한 풀 매니저. 게임 모드에서 사용
//                
//                
//----------------------------------------------------------------------------------------------------------

UCLASS()
class PROJECTSEOULZ_API USZPoolManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// 필요한 오브젝트를 미리 만들어놓는 함수
	void Pooling(TSubclassOf<AActor> ActorClass, int32 Count);

	// 오브젝트 풀에 있는 액터를 가져와 사용
	AActor* GetActor(TSubclassOf<AActor> ActorClass, const FTransform& SpawnTransform);

	// 풀에 액터 반환
	void ReturnActor(AActor* Actor);

	// 스테이지 종료 시 불필요한 풀 정리
	void CleanupForNextStage(const TArray<TSubclassOf<AActor>>& StageActorList);

private:
	// 액터의 종류에 따라 액터들을 저장하는 맵
	TMap<TSubclassOf<AActor>, TArray<AActor*>> PoolMap;

	// 변수로 받은 액터를 생성하는 함수. 특정 타입의 액터의 수가 부족하면 호출함
	AActor* CreateNewActor(TSubclassOf<AActor> ActorClass);
	
};
