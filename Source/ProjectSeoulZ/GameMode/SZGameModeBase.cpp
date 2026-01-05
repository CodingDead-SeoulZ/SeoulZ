// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SZGameModeBase.h"
#include "SZGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/SZPoolManager.h"

void ASZGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    PoolManager = GetGameInstance()->GetSubsystem<USZPoolManager>();

    // 스포너를 담을 배열. 특정 클래스가 들어가지 않기 때문에 Actor로 선언.
    TArray<AActor*> FoundActors;

    // 월드에 있는 SZMonsterSpawner를 FoundActors에 넣음.
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        ASZMonsterSpawner::StaticClass(),
        FoundActors
    );

    // Spawner들을 캐스팅해서 넣을 변수 변경.
    Spawners.Empty();

    // 캐스팅해서 Spawners에 저장
    for (AActor* Actor : FoundActors)
    {
        if (ASZMonsterSpawner* Spawner = Cast<ASZMonsterSpawner>(Actor))
        {
            Spawners.Add(Spawner);
        }
    }
    
    // 몬스터 스폰.
    SpawnStageMonsters();
}

void ASZGameModeBase::SpawnStageMonsters()
{
    // 스폰해서 생길 몬스터(살아날) 몬스터의 수를 위한 변수
    AliveMonsterCount = 0;

    // Spawners의 변수 수만큼 반복
    for (ASZMonsterSpawner* Spawner : Spawners)
    {
        // Spawner가 nullptr이면 종료.
        if (!Spawner) continue;
 
        // PoolManager를 매개변수로 받아 Spawner의 SpawnAll 호출.
        Spawner->SpawnAll(PoolManager);
        // 생성된 수만 큼 AliveMonsterCount 변수에 더해줌.
        AliveMonsterCount += Spawner->GetSpawnCount();
    }

    UE_LOG(LogTemp, Log, TEXT("Stage Start - Alive Monsters: %d"), AliveMonsterCount);
}
