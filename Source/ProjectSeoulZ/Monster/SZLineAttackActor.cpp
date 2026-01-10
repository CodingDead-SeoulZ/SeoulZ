// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/SZLineAttackActor.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/TimelineComponent.h"
#include "Components/CapsuleComponent.h"

ASZLineAttackActor::ASZLineAttackActor()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	static ConstructorHelpers::FClassFinder<UGameplayEffect> BPGE_RADamage(TEXT("/Game/Blueprints/GAS/Monster/GE/BPGE_RakeAttackDamage.BPGE_RakeAttackDamage_C"));
	if (BPGE_RADamage.Class)
	{
		GameplayEffectClass = BPGE_RADamage.Class;
	}
}

UAbilitySystemComponent* ASZLineAttackActor::GetAbilitySystemComponent() const
{
	return ASC;
}

//void ASZLineAttackActor::NotifyActorBeginOverlap(AActor* OtherActor)
//{
//	Super::NotifyActorBeginOverlap(OtherActor);
//	APawn* Pawn = Cast<APawn>(OtherActor);
//
//	UE_LOG(LogTemp, Log, TEXT("RA Attack Ovelap Ok!"));
//
//	if (Pawn && Pawn->IsPlayerControlled())
//	{
//		ApplyEffectToPlayer(OtherActor);
//	}
//}


void ASZLineAttackActor::OnSpawnFromPoolBlueprint_Implementation()
{
	OnSpawnFromPool_Implementation();
}

void ASZLineAttackActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this ) return;

	OverlappingActors.Add(OtherActor);
	UE_LOG(LogTemp, Log, TEXT("BeginOverlap"));
	CheckDamage();
}

void ASZLineAttackActor::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) return;

	OverlappingActors.Remove(OtherActor);
	UE_LOG(LogTemp, Log, TEXT("EndOverlap"));
}


void ASZLineAttackActor::BeginPlay()
{
	Super::BeginPlay();

	Hit->OnComponentBeginOverlap.AddDynamic(
		this, &ASZLineAttackActor::OnBeginOverlap);

	Hit->OnComponentEndOverlap.AddDynamic(
		this, &ASZLineAttackActor::OnEndOverlap);


	GetWorldTimerManager().SetTimer(
		DamageTimerHandle,
		this,
		&ASZLineAttackActor::CheckDamage,
		DamageInterval,
		true
	);

}

void ASZLineAttackActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ASC->InitAbilityActorInfo(this, this);
}

void ASZLineAttackActor::ApplyEffectToPlayer(AActor* Target)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC)
	{
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
		if (EffectSpecHandle.IsValid())
		{
			TargetASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

void ASZLineAttackActor::OnSpawnFromPool_Implementation()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	if (NiagaraEffect)
	{
		NiagaraEffect->DeactivateImmediate(); // 기존 이펙트 끄기
		NiagaraEffect->Activate(true);        // 재생 시작
	}

	FTimerHandle ReturnHandle;
	GetWorld()->GetTimerManager().SetTimer(
		ReturnHandle,
		this,
		&ASZLineAttackActor::OnReturnToPool_Implementation,
		3.0f,
		false
	);
}

void ASZLineAttackActor::OnReturnToPool_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ASZLineAttackActor::CheckDamage()
{
	if (OverlappingActors.Num() == 0)
	{
		return;
	}

	for (AActor* Target : OverlappingActors)
	{ 
		if (!IsValid(Target)) continue;
		APawn* Pawn = Cast<APawn>(Target);
		

		FVector TargetLocation = Target->GetActorLocation();
		FVector ReferenceLocation = ReferencePoint->GetComponentLocation();

		FVector TargetXY = FVector(TargetLocation.X, TargetLocation.Y, 0.f);
		FVector ReferenceXY = FVector(ReferenceLocation.X, ReferenceLocation.Y, 0.f);

		float DistanceXY = FVector::Dist(TargetXY, ReferenceXY);

		float Distance = FVector::Dist(TargetLocation, ReferenceLocation);
		float CurrentLength = Length * Progress;

		UE_LOG(LogTemp, Log, TEXT("Progress : %f"), Progress);
		UE_LOG(LogTemp, Log, TEXT("Distance : %f"), Distance);
		UE_LOG(LogTemp, Log, TEXT("CurrentLength : %f"), CurrentLength);

		if (Distance <= CurrentLength && (Pawn->IsPlayerControlled()))
		{

			UE_LOG(LogTemp, Log, TEXT("RA Attack Overlap Ok!"));

			ApplyEffectToPlayer(Target);
			
		}

		//float InputKey = SplineComp->FindInputKeyClosestToWorldLocation(ActorLocation);
		//FVector ClosestSplineLoc =
		//	SplineComp->GetLocationAtSplineInputKey(
		//		InputKey,
		//		ESplineCoordinateSpace::World
		//	);

		//float Distance = FVector::Dist(ActorLocation, ClosestSplineLoc);

		//// 스플라인 경로 밖이면 데미지
		//if (Distance <= ValidDistanceFromSpline)
		//{
		//	UGameplayStatics::ApplyDamage(
		//		Target,
		//		DamageAmount,
		//		GetInstigatorController(),
		//		this,
		//		nullptr
		//	);
		//}
	}

}













