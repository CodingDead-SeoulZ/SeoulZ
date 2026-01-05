// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/GA/SZGA_BansheeRakeAttack.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Monster/SZBossBanshee.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AI/BTTask_RakeAttack.h"
#include "AIController.h"
#include "AI/SZBossAIController.h"

USZGA_BansheeRakeAttack::USZGA_BansheeRakeAttack()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Monster/AM_RakeAttack.AM_RakeAttack'"));
	if (AttackMontageRef.Object)
	{
		AttackMontage = AttackMontageRef.Object;
	}
}

void USZGA_BansheeRakeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!AttackMontage)
	{
		return;
	}
	
	ASZBossBanshee* Banshee = Cast<ASZBossBanshee>(ActorInfo->AvatarActor.Get());
	if (!Banshee)
	{
		return;
	}
	
	//Banshee->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	Banshee->SetCurrentSkillAbility(this);
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SpawnSkill"), AttackMontage, 1.0f);
	PlayAttackTask->OnCompleted.AddDynamic(this, &USZGA_BansheeRakeAttack::OnCompleteCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &USZGA_BansheeRakeAttack::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();
}

void USZGA_BansheeRakeAttack::CancelAbility(const FGameplayAbilitySpecHandle Handle,
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

void USZGA_BansheeRakeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
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
	
	if (AAIController* AICon = Cast<AAIController>(Banshee->GetController()))
	{
		AICon->GetBrainComponent()->ResumeLogic(TEXT("RootMotion"));
		
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
			UBTTask_RakeAttack* MonsterRateAttackTask = Cast<UBTTask_RakeAttack>(Boss->GetCurrentTask());
			if (MonsterRateAttackTask)
			{
				MonsterRateAttackTask->OnSkillFinished(true);
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
			UBTTask_RakeAttack* MonsterRateAttackTask = Cast<UBTTask_RakeAttack>(Boss->GetCurrentTask());
			if (MonsterRateAttackTask)
			{
				MonsterRateAttackTask->OnSkillFinished(true);
			}
			
		}
	}

	//
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
