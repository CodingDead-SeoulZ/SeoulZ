// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SZItemBase.h"
#include "Components/SphereComponent.h"

// Sets default values
ASZItemBase::ASZItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(StaticMeshComp);
	// 충돌은 있지만 물리 시뮬레이션은 안 함. (회전 안 하도록)
	StaticMeshComp->SetSimulatePhysics(false);
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	ItemCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ItemCollision"));
	ItemCollision->SetupAttachment(StaticMeshComp);
	ItemCollision->InitSphereRadius(50.f);
	ItemCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	SZItemData = CreateDefaultSubobject<USZItemDataComp>(TEXT("SZItemData"));
}

const FName ASZItemBase::GetItemID() const 
{
	return ItemDataHandle.RowName;
}

const FItemTemplete* ASZItemBase::GetItemData() const
{
	return ItemDataHandle.GetRow<FItemTemplete>(TEXT("GetItemData"));
}

FText ASZItemBase::OnLookAt_Implementation() const
{
	if (const FItemTemplete* Data = GetItemData())
	{
		return FText::Format(FText::FromString(TEXT("{0} 줍기")), Data->Name);
	}
	return FText::GetEmpty();
}

void ASZItemBase::SetStaticMesh()
{
	if (GetItemData()) 
	{
		TObjectPtr<UStaticMesh> SM = GetItemData()->ItemMesh.StaticMesh;
		if (SM)
		{
			StaticMeshComp->SetStaticMesh(SM);
		}
	}
}

void ASZItemBase::SetMaterial()
{
}

void ASZItemBase::SpawnInitialItem()
{
	SetStaticMesh();

	if (StaticMeshComp)
	{
		// 회전 안 함.
		StaticMeshComp->SetSimulatePhysics(false);
		StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

// Called when the game starts or when spawned
void ASZItemBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASZItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

