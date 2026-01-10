// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RakeAttack.h"
#include "AIController.h"
#include "Monster/SZBossBanshee.h"
#include "GameplayAbilitySpec.h"
#include "AbilitySystemComponent.h"
#include "Monster/GA/SZGA_BansheeRakeAttack.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RakeAttack::UBTTask_RakeAttack()
{
}

EBTNodeResult::Type UBTTask_RakeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	ASZBossBanshee* Monster = Cast<ASZBossBanshee>(ControllingPawn);

	FGameplayAbilitySpec* SkillGASpec = Monster->GetAbilitySystemComponent()->FindAbilitySpecFromClass(USZGA_BansheeRakeAttack::StaticClass());
	
	if (!SkillGASpec)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability not yet given. Retrying next tick."));
		return EBTNodeResult::Failed; // Task 대기
	}
	
	if (!SkillGASpec->IsActive())
	{
		Monster->SetCurrentTask(this);
		UE_LOG(LogTemp,Log,TEXT("SkillGASpec is active."));
		Monster->GetAbilitySystemComponent()->TryActivateAbility(SkillGASpec->Handle);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability is a."));
	}

	CachedOwnerComp = &OwnerComp;
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool("bHasChasedPlayer", false);

	return EBTNodeResult::InProgress;
	
}

void UBTTask_RakeAttack::OnSkillFinished(bool bSuccess)
{
	if (!CachedOwnerComp)
	{
		return ;
	}

	// InProgress로 실행중인 Task를 끝내는 함수.
	FinishLatentTask(
		*CachedOwnerComp, bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed
	);
}
