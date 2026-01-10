// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SZNormalAIController.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 이혜성 
// Date         : 2025-12-00
// Copyright    : CodingDead
//
// Description  : 일반 몬스터가 사용하는 AI 컨트롤러.
//                
//                
//----------------------------------------------------------------------------------------------------------

UCLASS()
class PROJECTSEOULZ_API ASZNormalAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASZNormalAIController();

	// AI를 시작하는 함수.
	void RunAI();
	// AI를 멈추는 함수.
	void StopAI();

	// 해당 컨트롤러가 폰에 빙의될 때 호출되는 함수.
	virtual void OnPossess(APawn* InPawn) override;


private:
	// 사용할 블랙보드 
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	// 사용할 비헤이비어 트리
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
