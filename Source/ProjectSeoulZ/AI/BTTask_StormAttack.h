// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StormAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSEOULZ_API UBTTask_StormAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_StormAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnStormSkillFinished(bool bSuccess);

private:
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;
	
};
