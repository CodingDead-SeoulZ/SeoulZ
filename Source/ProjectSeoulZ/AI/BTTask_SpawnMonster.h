// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SpawnMonster.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSEOULZ_API UBTTask_SpawnMonster : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SpawnMonster();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UFUNCTION()
	void OnAttackFinished(bool bSuccess);

private:
	
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;
};
