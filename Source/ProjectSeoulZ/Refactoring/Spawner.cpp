// Fill out your copyright notice in the Description page of Project Settings.


#include "Refactoring/Spawner.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Player/SZCharacterPlayer.h"
#include "Refactoring/PoolManager.h"
#include "Item/SZItemBase.h"

#include "Components/SphereComponent.h"

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

FTransform ASpawner::MakeItemSpawnTransform() const
{
	// 랜덤 XY
	const float Radius = 200.0f; 
	const FVector2D Rand2D = FMath::RandPointInCircle(Radius);

	FVector Loc = GetActorLocation();
	Loc.X += (100 + Rand2D.X);
	Loc.Y += (100 + Rand2D.Y);

	// 회전은 고정
	const FRotator Rot(0, FMath::RandRange(0.f, 360.f), 0);
	return FTransform(Rot, Loc, FVector::OneVector);
}

FName ASpawner::GetRandomItemID() const
{
	if (!ItemDataTable)
	{
		return NAME_None;
	}

	TArray<FName> RowNames = ItemDataTable->GetRowNames();
	if (RowNames.Num() == 0)
	{
		return NAME_None;
	}

	const int32 Index = FMath::RandRange(0, RowNames.Num() - 1);
	const FName RandomItemID = RowNames[Index];
	return RandomItemID;
}

void ASpawner::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	ASZCharacterPlayer* Player = Cast<ASZCharacterPlayer>(OtherActor);
	if (!Player) { return; }

	UGameInstance* GI = GetGameInstance();
	if (!GI) { return; }

	UPoolManager* PoolManager = GI->GetSubsystem<UPoolManager>();
	if (!PoolManager) { return; }

	for (int i = 0; i < InitialSpawnCount; ++i)
	{
		const FTransform& SpawnTransform = MakeItemSpawnTransform();

		// 풀 매니저에서 아이템 액터 스폰
		APoolableActor* SpawnedActor = PoolManager->OnSpawn(ASZItemBase::StaticClass(), SpawnTransform);
		SpawnedActor->SetActorEnableCollision(false);
		SpawnedActor->SetActorTransform(SpawnTransform, false, nullptr, ETeleportType::TeleportPhysics);
		SpawnedActor->SetActorEnableCollision(true);

		// 스폰 후, 어떤 아이템인지 결정
		if (ASZItemBase* SpawnedItem = Cast<ASZItemBase>(SpawnedActor))
		{
			FName RandomItemID = GetRandomItemID();
			SpawnedItem->ItemDataHandle.DataTable = ItemDataTable;
			SpawnedItem->ItemDataHandle.RowName = RandomItemID;

			SpawnedItem->SpawnInitialItem();
		}
	}
}