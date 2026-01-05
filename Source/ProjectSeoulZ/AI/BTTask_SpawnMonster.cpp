// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SpawnMonster.h"
#include "AIController.h"
#include "GameplayAbilitySpec.h"
#include "Monster/GA/SZGA_BansheeMonsterSpawn.h"
#include "Monster/SZBossBanshee.h"
#include "AbilitySystemComponent.h"

UBTTask_SpawnMonster::UBTTask_SpawnMonster()
{
}

EBTNodeResult::Type UBTTask_SpawnMonster::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ASZBossBanshee* Monster = Cast<ASZBossBanshee>(ControllingPawn);

	FGameplayAbilitySpec* SkillGASpec = Monster->GetAbilitySystemComponent()->FindAbilitySpecFromClass(USZGA_BansheeMonsterSpawn::StaticClass());
	/*if (!SkillGASpec)
	{
		UE_LOG(LogTemp, Log, TEXT("Ability Error"));
		return EBTNodeResult::Failed;
	}*/
	if (!SkillGASpec)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Ability not yet given. Retrying next tick."));
		return EBTNodeResult::Failed; // Task 대기
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Ability given." ));
	}



	// ���� �����Ƽ ����.
	if (!SkillGASpec->IsActive())
	{
		Monster->SetCurrentTask(this);
		Monster->GetAbilitySystemComponent()->TryActivateAbility(SkillGASpec->Handle);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability is a."));
	}

	CachedOwnerComp = &OwnerComp;

	return EBTNodeResult::InProgress;
}

void UBTTask_SpawnMonster::OnAttackFinished(bool bSuccess)
{
	// 자기 자신을 할당되어 있어야만 태스크 구분 가능.
	if (!CachedOwnerComp)
	{
		return ;
	}

	// InProgress로 실행중인 Task를 끝내는 함수.
	FinishLatentTask(
		*CachedOwnerComp, bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed
	);
}
