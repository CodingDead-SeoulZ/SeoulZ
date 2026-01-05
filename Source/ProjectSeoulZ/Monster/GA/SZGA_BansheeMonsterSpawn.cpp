// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/GA/SZGA_BansheeMonsterSpawn.h"
#include "Monster/SZBossBanshee.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AI/BTTask_SpawnMonster.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Monster/SZNormalMonster.h"

USZGA_BansheeMonsterSpawn::USZGA_BansheeMonsterSpawn()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	static ConstructorHelpers::FClassFinder<AActor> BPMonsterClass(TEXT("/Game/Blueprints/BP_NormalUndead.BP_NormalUndead_C"));
	if (BPMonsterClass.Class)
	{
		SpawnClass = BPMonsterClass.Class;
	}
	//SpawnClass = ASZNormalMonster::StaticClass();
}

void USZGA_BansheeMonsterSpawn::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ASZBossBanshee* Banshee = Cast<ASZBossBanshee>(ActorInfo->AvatarActor.Get());
	if (!Banshee)
	{
		return;
	}

	SkillMontage = Banshee->GetSpawnSkillMontage();

	if (!SkillMontage)
	{
		return;
	}

	Banshee->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UE_LOG(LogTemp, Log, TEXT("Monster Spawn Ability OK"));

	Banshee->SetCurrentSkillAbility(this);
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SpawnSkill"), SkillMontage, 1.0f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &USZGA_BansheeMonsterSpawn::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &USZGA_BansheeMonsterSpawn::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

}

void USZGA_BansheeMonsterSpawn::CancelAbility(const FGameplayAbilitySpecHandle Handle,
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

void USZGA_BansheeMonsterSpawn::EndAbility(const FGameplayAbilitySpecHandle Handle,
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

void USZGA_BansheeMonsterSpawn::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	//
	bool bWasCancelled = false;

	if (ASZBossBanshee* Boss = Cast<ASZBossBanshee>(GetAvatarActorFromActorInfo()))
	{
		if (Boss->GetCurrentSkillAbility())
		{
			UBTTask_SpawnMonster* SpawnMonsterTask = Cast<UBTTask_SpawnMonster>(Boss->GetCurrentTask());
			if (SpawnMonsterTask)
			{
				SpawnMonsterTask->OnAttackFinished(true);
			}
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);

}

void USZGA_BansheeMonsterSpawn::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	//
	bool bWasCancelled = true;
	
	if (ASZBossBanshee* Boss = Cast<ASZBossBanshee>(GetAvatarActorFromActorInfo()))
	{
		if (Boss->GetCurrentSkillAbility())
		{
			UBTTask_SpawnMonster* SpawnMonsterTask = Cast<UBTTask_SpawnMonster>(Boss->GetCurrentTask());
			if (SpawnMonsterTask)
			{
				SpawnMonsterTask->OnAttackFinished(true);
			}
		}
	}

	//
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);

}
