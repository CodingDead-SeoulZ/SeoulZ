// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PoolManager.generated.h"

class APoolableActor;

UCLASS()
class PROJECTSEOULZ_API UPoolManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
    APoolableActor* OnSpawn();
    void OnReturn(APoolableActor* PoolableActor);

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
    TQueue<APoolableActor*> Pool;

    UPROPERTY(EditDefaultsOnly, Category = "Object Pool")
    TSubclassOf<APoolableActor> PoolableActorClass;

    UPROPERTY(EditDefaultsOnly, Category = "Object Pool")
    int32 InitPoolSize = 100;
};
