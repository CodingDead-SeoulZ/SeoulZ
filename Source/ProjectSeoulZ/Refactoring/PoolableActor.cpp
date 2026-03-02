// Fill out your copyright notice in the Description page of Project Settings.


#include "Refactoring/PoolableActor.h"
#include "Refactoring/PoolManager.h"

// Sets default values
APoolableActor::APoolableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APoolableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APoolableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APoolableActor::OnSpawn_Implementation()
{
	SetActorHiddenInGame(false);

	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void APoolableActor::OnReturn_Implementation()
{
	SetActorHiddenInGame(true);

	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void APoolableActor::ReturnToPool()
{
	UPoolManager* PoolManager = GetGameInstance()->GetSubsystem<UPoolManager>();
	if (PoolManager)
	{
		PoolManager->OnReturn(this);
	}
}

