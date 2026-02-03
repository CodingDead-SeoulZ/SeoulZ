// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/GA/SZGA_BansheeRakeAttack.h"
#include "SZGA_BansheeRakeAttack.h"
#include "Monster/SZBossBanshee.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AI/BTTask_RakeAttack.h"
#include "GameMode/SZPoolManager.h"
#include "Components/CapsuleComponent.h"
#include "Interface/SZPoolableInterface.h"
#include "Components/BoxComponent.h"
#include "Monster/SZLineAttackActor.h"

USZGA_BansheeRakeAttack::USZGA_BansheeRakeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Game/Animation/Monster/AM_RakeAttack.AM_RakeAttack"));
	if (AttackMontageRef.Object)
	{
	
		AttackMontage = AttackMontageRef.Object;
	}else
	{
		UE_LOG(LogTemp,Log,TEXT("Init fail"));
	}

	static ConstructorHelpers::FClassFinder<AActor> BPEffectClass(TEXT("/Game/Effect/_SplineVFX/_GenericSource/BP/BP_SplineVFX_Fire.BP_SplineVFX_Fire_C"));
	if (BPEffectClass.Class)
	{
		SpawnClass = BPEffectClass.Class;
	}

	SpawnCount = 2;

}

void USZGA_BansheeRakeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UE_LOG(LogTemp,Log,TEXT("BansheeRakeAttack"));
	
	ASZBossBanshee* Banshee = Cast<ASZBossBanshee>(ActorInfo->AvatarActor.Get());
	if (!Banshee)
	{
		UE_LOG(LogTemp,Error,TEXT("Banshee is null"));
		return;
	}

	Banshee->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UE_LOG(LogTemp, Log, TEXT("LineAttack Spawn Ability OK"));

	Banshee->SetCurrentSkillAbility(this);
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SpawnSwamp"), AttackMontage, 1.0f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &USZGA_BansheeRakeAttack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &USZGA_BansheeRakeAttack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();
}

void USZGA_BansheeRakeAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	ASZBossBanshee* Banshee = Cast<ASZBossBanshee>(ActorInfo->AvatarActor.Get());
	if (Banshee)
	{
		Banshee->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void USZGA_BansheeRakeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ASZBossBanshee* Banshee = Cast<ASZBossBanshee>(ActorInfo->AvatarActor.Get());
	if (Banshee)
	{
		Banshee->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	if (Banshee)
	{
		Banshee->SetCurrentSkillAbility(nullptr);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USZGA_BansheeRakeAttack::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	//
	bool bWasCancelled = false;

	if (ASZBossBanshee* Boss = Cast<ASZBossBanshee>(GetAvatarActorFromActorInfo()))
	{
		if (Boss->GetCurrentSkillAbility())
		{
			UBTTask_RakeAttack* SpawnMonsterTask = Cast<UBTTask_RakeAttack>(Boss->GetCurrentTask());
			if (SpawnMonsterTask)
			{
				SpawnMonsterTask->OnSkillFinished(true);
			}
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void USZGA_BansheeRakeAttack::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	//
	bool bWasCancelled = true;
	
	if (ASZBossBanshee* Boss = Cast<ASZBossBanshee>(GetAvatarActorFromActorInfo()))
	{
		if (Boss->GetCurrentSkillAbility())
		{
			UBTTask_RakeAttack* SpawnMonsterTask = Cast<UBTTask_RakeAttack>(Boss->GetCurrentTask());
			if (SpawnMonsterTask)
			{
				SpawnMonsterTask->OnSkillFinished(true);
			}
		}
	}

	//
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}


void USZGA_BansheeRakeAttack::SpawnActors(class USZPoolManager* PoolManager)
{
	UE_LOG(LogTemp, Log, TEXT("Spawning Actors in SkillMontage"));
	AActor* Owner = GetAvatarActorFromActorInfo();
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor info is null"));
		return;
	}

	UWorld* World = Owner->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World info is null"));
		return;
	}

	FVector BansheeForward = Owner->GetActorForwardVector();
	float MoveDistance = 200.0f;

	FVector SpawnLocation =
		Owner->GetActorLocation() +
		BansheeForward * 200.f;

	FRotator SpawnRotation = Owner->GetActorRotation();

	for (int i = 0; i <= SpawnCount; ++i)
	{
		FTransform SpawnTransform;
		SpawnRotation = Owner->GetActorRotation();
		//
		AActor* Actor = PoolManager->GetActor(SpawnClass, SpawnTransform);

		if (!Actor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor is null"));
			return;
		}

		//
		if (Actor->GetClass()->ImplementsInterface(USZPoolableInterface::StaticClass()))
		{
			//
			UE_LOG(LogTemp, Warning, TEXT("Spawn Ok!"));
			//
			if (i == 1)
			{
				UE_LOG(LogTemp, Log, TEXT("1 Turn"));
				SpawnRotation = SpawnRotation + FRotator(0,30,0);
			}
			else if (i == 2)
			{
				UE_LOG(LogTemp, Log, TEXT("2 Turn"));
				SpawnRotation = SpawnRotation + FRotator(0, -30, 0);
			}

			//FVector MovedDirection = SpawnRotation.RotateVector(BansheeForward).GetSafeNormal();
			FVector MovedDirection = SpawnRotation.Vector().GetSafeNormal();
			FVector MoveVector = MovedDirection * MoveDistance;

			ASZLineAttackActor* LineAttack = Cast<ASZLineAttackActor>(Actor);

			Actor->SetActorLocationAndRotation(
				SpawnLocation +MoveVector,
				SpawnRotation,
				false,   // bSweep
				nullptr,
				ETeleportType::TeleportPhysics
			);

			// 나중에 이걸로 바꿔야 함.
			LineAttack->OnSpawnFromPoolBlueprint();
			//ISZPoolableInterface::Execute_OnSpawnFromPool(Actor);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn Not Ok!"));

		}
	}

	// ✅ Transform 설정
	
}


