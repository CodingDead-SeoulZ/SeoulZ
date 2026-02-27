// Fill out your copyright notice in the Description page of Project Settings.


#include "Refactoring/PoolManager.h"
#include "Refactoring/PoolableActor.h"

APoolableActor* UPoolManager::OnSpawn()
{
    APoolableActor* PoolableActor = nullptr;
    if (Pool.IsEmpty())
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        PoolableActor = GetWorld()->SpawnActor<APoolableActor>(PoolableActorClass, SpawnParams);
    }
    else
    {
        Pool.Dequeue(PoolableActor);
    }

    if (PoolableActor)
    {
        PoolableActor->OnSpawn();
    }
    return PoolableActor;
}

void UPoolManager::OnReturn(APoolableActor* PoolableActor)
{
    if (PoolableActor)
    {
        IPoolable::Execute_OnReturn(PoolableActor);
        Pool.Enqueue(PoolableActor);
    }
}

void UPoolManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (PoolableActorClass)
    {
        for (int32 i = 0; i < InitPoolSize; ++i)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            APoolableActor* NewObject = GetWorld()->SpawnActor<APoolableActor>(PoolableActorClass, SpawnParams);
            if (NewObject)
            {
                IPoolable::Execute_OnReturn(NewObject);
                Pool.Enqueue(NewObject);
            }
        }
    }
}
