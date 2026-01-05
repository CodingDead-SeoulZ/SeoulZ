// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle/SZSwamp.h"

#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

// Sets default values
ASZSwamp::ASZSwamp()
{
	ASC=CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Swamp.Swamp'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	
	static ConstructorHelpers::FClassFinder<UGameplayEffect> GERef(TEXT("/Game/Blueprints/GAS/Monster/GE/BPGE_SwampDamage.BPGE_SwampDamage_C"));
	if (GERef.Class)
	{
		GameplayEffectClass = 	GERef.Class;
	}
	
	Mesh->SetRelativeScale3D(FVector(0.85f));
	Mesh->SetRelativeLocation(FVector(0.0f, -0.0f, 1.f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	
}

class UAbilitySystemComponent* ASZSwamp::GetAbilitySystemComponent() const
{
	return ASC;
}

void ASZSwamp::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	APawn* Pawn = Cast<APawn>(OtherActor);
	
	if (Pawn && Pawn->IsPlayerControlled())
	{
		ApplyEffectToTarget(OtherActor);	
	}
	
}

void ASZSwamp::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ASC->InitAbilityActorInfo(this,this);
}

void ASZSwamp::ApplyEffectToTarget(AActor* Target)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC)
	{
		FGameplayEffectContextHandle EffectContext =  TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass,1,EffectContext);
		if (EffectSpecHandle.IsValid())
		{
			TargetASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

void ASZSwamp::OnSpawnFromPool_Implementation()
{
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	
}

void ASZSwamp::OnReturnToPool_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}




