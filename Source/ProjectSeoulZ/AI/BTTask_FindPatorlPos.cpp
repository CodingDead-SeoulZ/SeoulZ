// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatorlPos.h"
#include "SZAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPatorlPos::UBTTask_FindPatorlPos()
{
}

// 노드를 실행. 이 함수를 구현해야 비헤이비어트리에서 노드 실행.
EBTNodeResult::Type UBTTask_FindPatorlPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AIController가 조종하고 있는 Pawn을 가져와서 검사함.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Pawn Fail"));
		return EBTNodeResult::Failed;
	}

	// 현재 월드에 존재하는 내비게이션 시스템을 가져오는 함수.
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Nav Fail"));
		return EBTNodeResult::Failed;
	}

	// 블랙보드에 저장되어 있는 현재 위치를 가져온다. 그리고 Origin에 할당.
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS);
	FNavLocation NextPatrolPos;

	// Origin으로 특정 범위 안에 있는 랜덤한 지점을 NextPatrolPos에 할당하는 함수. 성공하면 블랙보드의 BBKEY_PATROLPOS에 저장.
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 1500.0f, NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
