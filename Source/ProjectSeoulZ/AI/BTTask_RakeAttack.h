// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RakeAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSEOULZ_API UBTTask_RakeAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_RakeAttack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UFUNCTION()
	void OnSkillFinished(bool bSuccess);
	
private:
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;
};
