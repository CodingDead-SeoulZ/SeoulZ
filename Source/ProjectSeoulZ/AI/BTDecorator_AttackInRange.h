// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AttackInRange.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 이혜성
// Date         : 2025-12-25
// Copyright    : CodingDead
//
// Description  : 일반 좀비 몬스터에서 사용할 노드. 
//                공격 사거리 안에 들어오는 판단하는 노드.
//                
//                
//----------------------------------------------------------------------------------------------------------

UCLASS()
class PROJECTSEOULZ_API UBTDecorator_AttackInRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	//
	UBTDecorator_AttackInRange();

protected:
	//
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
