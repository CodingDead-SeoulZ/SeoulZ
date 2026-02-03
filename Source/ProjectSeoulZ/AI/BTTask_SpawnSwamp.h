// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SpawnSwamp.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSEOULZ_API UBTTask_SpawnSwamp : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SpawnSwamp();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UFUNCTION()
	void OnSkillFinished(bool bSuccess);
	
private:

	// ��������Ʈ���� �ڽ��� ������ �ѱ�� ���� ������ ȣ��Ǿ�� �ϴµ� �̸� ���� �ڱ� �ڽ��� �����ϱ� ���� ����
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;
};
