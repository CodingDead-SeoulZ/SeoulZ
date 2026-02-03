// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Interface/SZNormalAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
}

// 노드를 실행. 이 함수를 구현해야 비헤이비어트리에서 노드 실행.
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AIController가 조종하고 있는 Pawn을 가져와서 검사함.
	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	

	// 폰이 특정 인터페이스를 구현하고 있는지를 확인(몬스터의 데이터를 가져오는 인터페이스).
	ISZNormalAIInterface* AIPawn = Cast<ISZNormalAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 자기 자신을 저장.
	CachedOwnerComp = &OwnerComp;

	// 인터페이스에서 AttackByAi 호출, 호출하면서 종료시점을 받기 위해 자기 자신을 매개변수로 넘긴다.
	AIPawn->AttackByAI(this);

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::OnAttackFinished(bool bSuccess)
{
	// 자기 자신을 할당되어 있어야만 태스크 구분 가능.
	if (!CachedOwnerComp)
	{
		return;
	}

	// InProgress로 실행중인 Task를 끝내는 함수.
	FinishLatentTask(
		*CachedOwnerComp, bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed
	);
}
