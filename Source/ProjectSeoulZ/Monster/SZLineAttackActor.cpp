// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/SZLineAttackActor.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystemComponent.h"


//ASZLineAttackActor::ASZLineAttackActor(const FVector& InDirection)
//{
//
//	PrimaryActorTick.bCanEverTick = true;
//
//	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
//	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
//	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
//
//	RootComponent = CollisionBox;
//	NiagaraComp->SetupAttachment(RootComponent);
//
//	//ForwardDirec = InDirection;
//}

//ASZLineAttackActor::ASZLineAttackActor()
//{
//	PrimaryActorTick.bCanEverTick = true;
//
//	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
//	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
//	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
//
//	RootComponent = CollisionBox;
//	NiagaraComp->SetupAttachment(RootComponent);
//}

// Called when the game starts or when spawned
//void ASZLineAttackActor::BeginPlay()
//{
//	Super::BeginPlay();
//
//	// 나이아가라 UserParameter 초기화
//	NiagaraComp->SetVariableVec3(FName("User.ForwardVector"), Direction);
//	NiagaraComp->SetVariableFloat(FName("User.CurrentLength"), CurrentLength);
//	NiagaraComp->SetVariableFloat(FName("User.GrowthSpeed"), GrowthSpeed);
//	
//}
//
//void ASZLineAttackActor::OnSpawnFromPool_Implementation()
//{
//	SetActorHiddenInGame(false);
//	SetActorEnableCollision(true);
//}
//
//void ASZLineAttackActor::OnReturnToPool_Implementation()
//{
//	SetActorHiddenInGame(true);
//	SetActorEnableCollision(false);
//}

ASZLineAttackActor::ASZLineAttackActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	NiagaraComp->SetupAttachment(RootComponent);


}

void ASZLineAttackActor::Tick(float DeltaTime)
{
}

void ASZLineAttackActor::BeginPlay()
{
}







