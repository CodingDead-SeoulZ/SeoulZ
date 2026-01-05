// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Monster/SZMonsterSpawner.h"
#include "SZGameModeBase.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 
// Date         : 2025-12-00
// Copyright    : 
//
// Description  : 
//                
//                
//----------------------------------------------------------------------------------------------------------

UCLASS()
class PROJECTSEOULZ_API ASZGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
    
    //void SpawnMonster();
        
    //
    void SpawnStageMonsters();

    FORCEINLINE int32 GetMonsterCount() { return AliveMonsterCount; }
    FORCEINLINE void SetMonsterCount(int32 Count) { AliveMonsterCount = Count; }

    //
    FORCEINLINE USZPoolManager* GetPoolManager() { return PoolManager; }

protected:
    // 생성시킬 몬스터의 클래스
    /*UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AActor> MonsterClass;*/

    // 스폰 반경
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float SpawnRadius = 800.f;

    // 스포너들의 배열, 스포너를 여러개 두는 게 좋음.
    UPROPERTY()
    TArray<ASZMonsterSpawner*> Spawners;

    // 스폰되고 살아있는 몬스터의 수를 저장.
    int32 AliveMonsterCount = 0;

    // 오브젝트 풀링을 위한 PoolManager
    UPROPERTY()
    USZPoolManager* PoolManager;

	
};
