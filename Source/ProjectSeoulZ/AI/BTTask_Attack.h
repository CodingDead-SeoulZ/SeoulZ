// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 이혜성
// Date         : 2025-12-00
// Copyright    : CodingDead
//
// Description  : 일반 좀비 몬스터의 비헤이비어 트리에서 사용할 노드.
//                공격을 수행한다.
//                
//----------------------------------------------------------------------------------------------------------

UCLASS()
class PROJECTSEOULZ_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Attack();

	//
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 델리게이트 종료 시 호출되는 함수.
	UFUNCTION()
	void OnAttackFinished(bool bSuccess);

private:

	// 델리게이트에서 자신의 정보를 넘기고 종료 시점에 호출되어야 하는데 이를 위해 자기 자신을 저장하기 위한 변수
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;
	
};
