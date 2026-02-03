// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/GA/SZGA_BansheeStormSpawn.h"
#include "SZGA_BansheeStormSpawn.h"
#include "Monster/SZBossBanshee.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AI/BTTask_StormAttack.h"
#include "Interface/SZPoolableInterface.h"
#include "GameMode/SZPoolManager.h"

USZGA_BansheeStormSpawn::USZGA_BansheeStormSpawn()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SkillMontageRef(TEXT("/Game/Animation/Monster/AM_Storm.AM_Storm"));
	if (SkillMontageRef.Object)
	{

		SkillMontage = SkillMontageRef.Object;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Init fail"));
	}

	static ConstructorHelpers::FClassFinder<AActor> BPStormClass(TEXT("/Game/FXVarietyPack/Blueprints/BP_ky_storm.BP_ky_storm_C"));
	if (BPStormClass.Class)
	{
		SpawnClass = BPStormClass.Class;
	}

	SpawnCount = 2;
}

void USZGA_BansheeStormSpawn::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Log, TEXT("Spawn Storm Attack"));

	ASZBossBanshee* Banshee = Cast<ASZBossBanshee>(ActorInfo->AvatarActor.Get());
	if (!Banshee)
	{
		UE_LOG(LogTemp, Error, TEXT("Banshee is null"));
		return;
	}

	Banshee->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UE_LOG(LogTemp, Log, TEXT("Storm Ability"));
	Banshee->SetCurrentSkillAbility(this);
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SpawnStorm"), SkillMontage, 1.0f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &USZGA_BansheeStormSpawn::OnStormCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &USZGA_BansheeStormSpawn::OnStormInterruptedCallback);
	PlayAttackTask->ReadyForActivation();
}

void USZGA_BansheeStormSpawn::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	ASZBossBanshee* Banshee = Cast<ASZBossBanshee>(ActorInfo->AvatarActor.Get());
	if (Banshee)
	{
		Banshee->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void USZGA_BansheeStormSpawn::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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

void USZGA_BansheeStormSpawn::SpawnActors(USZPoolManager* PoolManager)
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
		BansheeForward * 50.f;

	SpawnLocation.Z = -10;

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
				SpawnRotation = SpawnRotation + FRotator(0, 30, 0);
			}
			else if (i == 2)
			{
				UE_LOG(LogTemp, Log, TEXT("2 Turn"));
				SpawnRotation = SpawnRotation + FRotator(0, -30, 0);
			}

			//FVector MovedDirection = SpawnRotation.RotateVector(BansheeForward).GetSafeNormal();
			FVector MovedDirection = SpawnRotation.Vector().GetSafeNormal();
			FVector MoveVector = MovedDirection * MoveDistance;


			Actor->SetActorLocationAndRotation(
				SpawnLocation + MoveVector,
				SpawnRotation,
				false,   // bSweep
				nullptr,
				ETeleportType::TeleportPhysics
			);

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn Not Ok!"));

		}
	}
}

void USZGA_BansheeStormSpawn::OnStormCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	//
	bool bWasCancelled = false;

	if (ASZBossBanshee* Boss = Cast<ASZBossBanshee>(GetAvatarActorFromActorInfo()))
	{
		if (Boss->GetCurrentSkillAbility())
		{
			UBTTask_StormAttack* SpawnStormTask = Cast<UBTTask_StormAttack>(Boss->GetCurrentTask());
			if (SpawnStormTask)
			{
				SpawnStormTask->OnStormSkillFinished(true);
			}
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void USZGA_BansheeStormSpawn::OnStormInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	//
	bool bWasCancelled = true;

	if (ASZBossBanshee* Boss = Cast<ASZBossBanshee>(GetAvatarActorFromActorInfo()))
	{
		if (Boss->GetCurrentSkillAbility())
		{
			UBTTask_StormAttack* SpawnStormTask = Cast<UBTTask_StormAttack>(Boss->GetCurrentTask());
			if (SpawnStormTask)
			{
				SpawnStormTask->OnStormSkillFinished(true);
			}
		}
	}

	//
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
