// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/GA/SZGA_BansheeSwampSpawn.h"
#include "Monster/SZBossBanshee.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AI/BTTask_SpawnSwamp.h"
#include "Obstacle/SZSwamp.h"

USZGA_BansheeSwampSpawn::USZGA_BansheeSwampSpawn()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	SpawnClass = ASZSwamp::StaticClass();
	
}

void USZGA_BansheeSwampSpawn::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UE_LOG(LogTemp,Log,TEXT("Spawning BansheeSwampSpawn"));
	
	ASZBossBanshee* Banshee = Cast<ASZBossBanshee>(ActorInfo->AvatarActor.Get());
	if (!Banshee)
	{
		UE_LOG(LogTemp,Error,TEXT("Banshee is null"));
		return;
	}

	SkillMontage = Banshee->GetSpawnSwampMontage();

	if (!SkillMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("SkillMontage is NULL"));
		return;
	}

	Banshee->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UE_LOG(LogTemp, Log, TEXT("Swamp Spawn Ability OK"));

	Banshee->SetCurrentSkillAbility(this);
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SpawnSwamp"), SkillMontage, 1.0f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &USZGA_BansheeSwampSpawn::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &USZGA_BansheeSwampSpawn::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();
}

void USZGA_BansheeSwampSpawn::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	ASZBossBanshee* Banshee = Cast<ASZBossBanshee>(ActorInfo->AvatarActor.Get());
	if (Banshee)
	{
		Banshee->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void USZGA_BansheeSwampSpawn::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
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

void USZGA_BansheeSwampSpawn::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	//
	bool bWasCancelled = false;

	if (ASZBossBanshee* Boss = Cast<ASZBossBanshee>(GetAvatarActorFromActorInfo()))
	{
		if (Boss->GetCurrentSkillAbility())
		{
			UBTTask_SpawnSwamp* SpawnMonsterTask = Cast<UBTTask_SpawnSwamp>(Boss->GetCurrentTask());
			if (SpawnMonsterTask)
			{
				SpawnMonsterTask->OnSkillFinished(true);
			}
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void USZGA_BansheeSwampSpawn::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	//
	bool bWasCancelled = true;
	
	if (ASZBossBanshee* Boss = Cast<ASZBossBanshee>(GetAvatarActorFromActorInfo()))
	{
		if (Boss->GetCurrentSkillAbility())
		{
			UBTTask_SpawnSwamp* SpawnMonsterTask = Cast<UBTTask_SpawnSwamp>(Boss->GetCurrentTask());
			if (SpawnMonsterTask)
			{
				SpawnMonsterTask->OnSkillFinished(true);
			}
		}
	}

	//
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
