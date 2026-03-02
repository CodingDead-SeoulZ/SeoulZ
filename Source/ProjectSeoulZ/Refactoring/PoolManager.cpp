// Fill out your copyright notice in the Description page of Project Settings.


#include "Refactoring/PoolManager.h"
#include "Refactoring/PoolableActor.h"

APoolableActor* UPoolManager::OnSpawn(TSubclassOf<APoolableActor> PoolableActorClass, const FTransform& SpawnTransform)
{
    if (!PoolableActorClass) {
        return nullptr;
    }

    APoolableActor* PoolableActor = nullptr;
    FPoolState& State = GetOrCreateState(PoolableActorClass);

#pragma region Dequeue
    while (!State.Pool.IsEmpty() && State.Size > 0)
    {
        TWeakObjectPtr<APoolableActor> Weak;

        // Dequeue 실패는 풀에 더 이상 액터가 없다는 뜻이므로 루프 종료
        if (!State.Pool.Dequeue(Weak)) {
            break;
        }

        // Dequeue 성공
        --State.Size;
        
        // 유효하다면 풀에서 꺼내서 사용, 그렇지 않다면 다음 액터를 시도
        if (Weak.IsValid())
        {
            PoolableActor = Weak.Get();
            break;
        }
    }
#pragma endregion

#pragma region Setting
    if (PoolableActor) {
        PoolableActor->SetActorTransform(SpawnTransform, false, nullptr, ETeleportType::TeleportPhysics);
    }
    else { 
        // 이미 최대치 만큼 사용
        if (State.MaxSize > 0 && State.SpawnedCount >= State.MaxSize) { 
            return nullptr;
        }

        // 새로 생성
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        PoolableActor = GetWorld()->SpawnActor<APoolableActor>(PoolableActorClass, SpawnTransform, SpawnParams);
    }
#pragma endregion

    if (PoolableActor) {
        IPoolable::Execute_OnSpawn(PoolableActor);
        ++State.SpawnedCount;
	}
    return PoolableActor;
}

void UPoolManager::OnReturn(APoolableActor* PoolableActor)
{
	if (!IsValid(PoolableActor)) { 
        return; 
    }

	const TSubclassOf<APoolableActor>  PoolableActorClass = PoolableActor->GetClass();
    FPoolState& State = GetOrCreateState(PoolableActorClass);

    IPoolable::Execute_OnReturn(PoolableActor);

    State.Pool.Enqueue(PoolableActor);
    ++State.Size;
	--State.SpawnedCount;
}

//void UPoolManager::Initialize(FSubsystemCollectionBase& Collection)
//{
//    Super::Initialize(Collection);
//
//    if (PoolableActorClass)
//    {
//        for (int32 i = 0; i < InitPoolSize; ++i)
//        {
//            FActorSpawnParameters SpawnParams;
//            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//            APoolableActor* NewObject = GetWorld()->SpawnActor<APoolableActor>(PoolableActorClass, SpawnParams);
//            if (NewObject)
//            {
//                IPoolable::Execute_OnReturn(NewObject);
//                Pool.Enqueue(NewObject);
//            }
//        }
//    }
//}
