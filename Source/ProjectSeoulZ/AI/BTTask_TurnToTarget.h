// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToTarget.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 이혜성
// Date         : 2025-12-00
// Copyright    : CodinDead
//
// Description  : 일반 좀비 몬스터의 비헤이비어트리에서 사용하는 노드.
//                공격 시 플레이어의 방향으로 이동하는 역할을 한다.
//                
//                
//----------------------------------------------------------------------------------------------------------

UCLASS()
class PROJECTSEOULZ_API UBTTask_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TurnToTarget();
	
	//
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
