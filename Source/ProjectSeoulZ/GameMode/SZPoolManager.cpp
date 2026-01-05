// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SZPoolManager.h"
#include "SZPoolManager.h"
#include "Interface/SZPoolableInterface.h"

void USZPoolManager::Pooling(TSubclassOf<AActor> ActorClass, int32 Count)
{
	// 풀에 해당 액터 풀이 존재하는지 확인 후 없으면 추가.
	TArray<AActor*>& Pool = PoolMap.FindOrAdd(ActorClass);

	// 현재 월드 가져오기
	UWorld* World = GetWorld();
	if (!World) return;

	// 필요한만큼 액터를 생성하고 풀에 추가
	for (int32 i = 0; i < Count; i++)
	{
		AActor* NewActor = CreateNewActor(ActorClass);
		Pool.Add(NewActor);
	}
}

AActor* USZPoolManager::GetActor(TSubclassOf<AActor> ActorClass, const FTransform& SpawnTransform)
{
	TArray<AActor*>& Pool = PoolMap.FindOrAdd(ActorClass);

	// 해당 액터 타입에 해당하는 풀의 액터를 확인
	for (AActor* Actor : Pool)
	{
		// 존재하는 액터이고 액터가 비활성인지 확인. 비활성이라면 실행
		if (Actor && Actor->IsHidden()) // 비활성 상태 확인
		{
			// 액터의 Transform 설정.
			Actor->SetActorTransform(SpawnTransform);
			// SZPoolableInterface를 선언한 액터의 OnSpawnFromPool 실행
			ISZPoolableInterface::Execute_OnSpawnFromPool(Actor);
			return Actor;
		}
	}

	// 부족하면 새로 생성
	AActor* NewActor = CreateNewActor(ActorClass);
	if (NewActor)
	{
		// 생성한 액터 풀에 추가
		Pool.Add(NewActor);
		// 액터의 위치 설정.
		NewActor->SetActorTransform(SpawnTransform);
		ISZPoolableInterface::Execute_OnSpawnFromPool(NewActor);
	}

	return NewActor;
}

void USZPoolManager::ReturnActor(AActor* Actor)
{
	// 액터가 존재하고 그 액터가 SZPoolableInterface를 구현하지 않았다면 return
	if (!Actor || !Actor->GetClass()->ImplementsInterface(USZPoolableInterface::StaticClass())) return;

	// 위의 조건이 아니라면 액터를 풀에 리턴.
	ISZPoolableInterface::Execute_OnReturnToPool(Actor);
}


// 이건 아직 사용 안함.
void USZPoolManager::CleanupForNextStage(const TArray<TSubclassOf<AActor>>& StageActorList)
{
	//
	for (auto It = PoolMap.CreateIterator(); It; ++It)
	{
		//
		if (!StageActorList.Contains(It.Key()))
		{
			//
			for (AActor* Actor : It.Value())
			{
				//
				if (Actor) Actor->Destroy();
			}
			It.RemoveCurrent();
		}
	}
}

AActor* USZPoolManager::CreateNewActor(TSubclassOf<AActor> ActorClass)
{
	//
	UWorld* World = GetWorld();
	if (!World || !*ActorClass) return nullptr;

	//
	FActorSpawnParameters Params;
	//
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//
	AActor* NewActor = World->SpawnActor<AActor>(ActorClass, FTransform::Identity, Params);

	//
	if (NewActor && NewActor->GetClass()->ImplementsInterface(USZPoolableInterface::StaticClass()))
	{
		//
		ISZPoolableInterface::Execute_OnReturnToPool(NewActor);
	}

	return NewActor;
}
