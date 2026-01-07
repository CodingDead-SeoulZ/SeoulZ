// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/SZWardrobe.h"
#include "Item/SZItemTemplete.h"

// Sets default values
ASZWardrobe::ASZWardrobe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character"));
	CharacterMesh->SetupAttachment(Root);

	// 1. 의상
	Helmet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Helmet"));
	Helmet->SetupAttachment(CharacterMesh);

	Vest = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Vest"));
	Vest->SetupAttachment(CharacterMesh);

	Gloves = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gloves"));
	Gloves->SetupAttachment(CharacterMesh);

	// 2. 무기
	Holster = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Holster"));
	Holster->SetupAttachment(CharacterMesh);

	Magazine = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Magazine"));
	Magazine->SetupAttachment(CharacterMesh);

	PrimaryGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PrimaryGun"));
	PrimaryGun->SetupAttachment(CharacterMesh);

	SecondaryGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SecondaryGun"));
	SecondaryGun->SetupAttachment(CharacterMesh);
}

void ASZWardrobe::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!IsValid(CharacterMesh))
	{
		return;
	}

	auto Equipment = [&](USkeletalMeshComponent* Follower)
		{
			if (!IsValid(Follower) || Follower == CharacterMesh)
			{
				return;
			}
			const bool bForceUpdate = true;
			const bool bFollowerShouldTickPose = false;

			Follower->SetLeaderPoseComponent(CharacterMesh, bForceUpdate, bFollowerShouldTickPose);
			// Follower->bUpdateAnimationInEditor = false;
			// Follower->SetAnimationMode(EAnimationMode::AnimationBlueprint); 
		};

	Equipment(Vest);
	Equipment(Gloves);
	Equipment(Holster);
	Equipment(Magazine);
}

void ASZWardrobe::SetSKM(EEquipmentSlotType SlotType, USkeletalMesh* NewMesh)
{
	GetPlayerPartBySlotType(SlotType)->SetSkeletalMesh(NewMesh);
}

void ASZWardrobe::ClearSKM(EEquipmentSlotType SlotType)
{
	GetPlayerPartBySlotType(SlotType)->SetSkeletalMesh(nullptr);
}

// Called when the game starts or when spawned
void ASZWardrobe::BeginPlay()
{
	Super::BeginPlay();
	
}

USkeletalMeshComponent* ASZWardrobe::GetPlayerPartBySlotType(EEquipmentSlotType SlotType) const
{
	switch (SlotType)
	{
	// 의상
	case EEquipmentSlotType::Helmet:     return Helmet;
	case EEquipmentSlotType::Vest:       return Vest;
	case EEquipmentSlotType::Gloves:     return Gloves;
	// 무기
	case EEquipmentSlotType::Holster:       return Holster;
	case EEquipmentSlotType::Magazine:      return Magazine;
	case EEquipmentSlotType::PrimaryGun:   return PrimaryGun;
	case EEquipmentSlotType::SecondaryGun: return SecondaryGun;
	default: return nullptr;
	}
}

// Called every frame
void ASZWardrobe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

