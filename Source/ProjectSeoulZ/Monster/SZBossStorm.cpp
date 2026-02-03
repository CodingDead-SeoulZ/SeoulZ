// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/SZBossStorm.h"

// Sets default values
ASZBossStorm::ASZBossStorm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASZBossStorm::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASZBossStorm::OnSpawnFromPool_Implementation()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void ASZBossStorm::OnReturnToPool_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

// Called every frame
void ASZBossStorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

