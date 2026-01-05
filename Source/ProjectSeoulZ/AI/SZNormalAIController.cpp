// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SZNormalAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/SZAI.h"

ASZNormalAIController::ASZNormalAIController()
{
	// 블랙보드 할당
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_Normal.BB_Normal'"));
	if (BBAssetRef.Object != nullptr)
	{
		BBAsset = BBAssetRef.Object;
	}

	// 비헤이비어 트리 할당
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_Normal.BT_Normal'"));
	if (BTAssetRef.Object != nullptr)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void ASZNormalAIController::RunAI()
{
	if (BTAsset)
	{
		UBlackboardComponent* BlackboardPtr = Blackboard.Get();
		if (UseBlackboard(BBAsset,BlackboardPtr))
		{
			// 블랙보드의 BBKEY_HOMEPOS에 생성되는 위치 저장
			Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

			// 비헤이비어 트리 시작
			bool RunResult = RunBehaviorTree(BTAsset);
			ensure(RunResult);
		}
	}
	
}

void ASZNormalAIController::StopAI()
{
	// 비헤이비어트리 멈춤
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

// 해당 컨트롤러가 빙의될 때 비헤이비어트리 시작.
void ASZNormalAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
