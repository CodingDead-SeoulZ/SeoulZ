// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SZBossAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSEOULZ_API ASZBossAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASZBossAIController();

	void RunAI();

	void StopAI();

	// 해당 컨트롤러가 폰에 빙의될 때 호출되는 함수.
	virtual void OnPossess(APawn* InPawn) override;

protected:
	// 사용할 블랙보드 
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	// 사용할 비헤이비어 트리
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
	
};
