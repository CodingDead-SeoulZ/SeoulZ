// Fill out your copyright notice in the Description page of Project Settings.


#include "Refactoring/Spawner.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Player/SZCharacterPlayer.h"
#include "Refactoring/PoolManager.h"
#include "Item/SZItemBase.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	// StaticMesh->SetupAttachment(Root);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASpawner::OnTriggerBeginOverlap);
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(OtherActor);
	if (!Player) { return; }

	UGameInstance* GI = GetGameInstance();
	if (!GI) { return; }

	UPoolManager* PoolManager = GI->GetSubsystem<UPoolManager>();
	if (!PoolManager) { return; }

	const FTransform& SpawnTransform = GetActorTransform();

	// 풀 매니저에서 아이템 액터 스폰
	APoolableActor* SpawnedActor = PoolManager->OnSpawn(ASZItemBase::StaticClass(), SpawnTransform);

	// 스폰 후, 어떤 아이템인지 결정
	if (ASZItemBase* SpawnedItem = Cast<ASZItemBase>(SpawnedActor))
	{
		FName RandomItemID = "0001";
		SpawnedItem->ItemDataHandle.DataTable = ItemDataTable;
		SpawnedItem->ItemDataHandle.RowName = RandomItemID;
	}
}