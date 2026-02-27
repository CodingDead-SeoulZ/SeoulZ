// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Refactoring/Poolable.h"
#include "PoolableActor.generated.h"

UCLASS()
class PROJECTSEOULZ_API APoolableActor : public AActor, public IPoolable
{ 
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoolableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// IPoolable interface implementation
    virtual void OnSpawn_Implementation() override;
    virtual void OnReturn_Implementation() override;

	// Return the projectile to the pool
	void ReturnToPool();
};
