// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SZPoolableInterface.h"
#include "SZBossStorm.generated.h"

UCLASS()
class PROJECTSEOULZ_API ASZBossStorm : public AActor, public ISZPoolableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASZBossStorm();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnSpawnFromPool_Implementation() override;

	//
	virtual void OnReturnToPool_Implementation() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
