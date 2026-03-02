// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PoolManager.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2026-03-02
// Copyright    : 
//
// Description : 오브젝트 풀 매니저
//               여러 PooalableActor들을 관리하는 역할
//  1. 각 클래스별로 풀링 설정을 관리하는 PoolConfig 구조체
//  2. 각 클래스별로 풀 상태를 관리하는 PoolState 구조체
//  3. 각 클래스에 맞는 풀링 설정과 상태에 따라, 각 액터병 생성과 반환을 담당
//				 예, 총알, 몬스터, 아이템 등 풀링이 필요한 액터들을 관리
//                 
//----------------------------------------------------------------------------------------------------------

class APoolableActor;

struct FPoolState
{
    TQueue<TWeakObjectPtr<APoolableActor>> Pool;

    // 미리 만들어 둘 개수
    // int32 InitSize = 32;

    // 현재 풀에 있는 액터 수
    int32 Size = 0;

	// 풀의 최대치
    int32 MaxSize = 200;

	// 스폰된 수
	int32 SpawnedCount = 0;
};

UCLASS()
class PROJECTSEOULZ_API UPoolManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
    APoolableActor* OnSpawn(TSubclassOf<APoolableActor> PoolableActorClass, const FTransform& SpawnTransform);
    void OnReturn(APoolableActor* PoolableActor);

protected:
    // virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
    FPoolState& GetOrCreateState(UClass* Key)
    {
        if (TUniquePtr<FPoolState>* Found = PoolState.Find(Key))
        {
            return *Found->Get();
        }

        TUniquePtr<FPoolState> NewState = MakeUnique<FPoolState>();
        FPoolState& Ref = *NewState;      
        PoolState.Add(Key, MoveTemp(NewState));
        return Ref;
    }

private:
    TMap<TSubclassOf<APoolableActor>, TUniquePtr<FPoolState>> PoolState;
};
