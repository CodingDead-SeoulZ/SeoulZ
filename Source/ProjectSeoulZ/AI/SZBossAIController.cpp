// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SZBossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/SZAI.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"


ASZBossAIController::ASZBossAIController()
{
	// 블랙보드 할당
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_Boss.BB_Boss'"));
	if (BBAssetRef.Object != nullptr)
	{
		BBAsset = BBAssetRef.Object;
	}

	// 비헤이비어 트리 할당
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_Boss.BT_Boss'"));
	if (BTAssetRef.Object != nullptr)
	{
		BTAsset = BTAssetRef.Object;
	}

	UPathFollowingComponent* PathComp = GetPathFollowingComponent();
	if (PathComp)
	{
		PathComp->Activate(true);  // 강제로 활성화
		PathComp->bAutoActivate = true; // 이후에도 자동 활성화
		UE_LOG(LogTemp, Log, TEXT("PathFollowingComponent activated in BeginPlay"));
	}


}

void ASZBossAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		// 블랙보드의 BBKEY_HOMEPOS에 생성되는 위치 저장
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			Blackboard->SetValueAsObject(BBKEY_TARGET, PlayerPawn);
		}
		else {
			UE_LOG(LogTemp, Log, TEXT("BB Target is nullptr"));
		}

		// 비헤이비어 트리 시작
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void ASZBossAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void ASZBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);


	RunAI();
}

void ASZBossAIController::BeginPlay()
{
	Super::BeginPlay();


	APawn* boss = GetPawn();
	GetWorld()->GetTimerManager().SetTimer(PlayerCheckTimerHandle, this, &ASZBossAIController::TrySetPlayerTarget, 0.2f, true);

}

void ASZBossAIController::TrySetPlayerTarget()
{
	if (!Blackboard) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		Blackboard->SetValueAsObject(BBKEY_TARGET, PlayerPawn);

		// 타이머 종료
		GetWorld()->GetTimerManager().ClearTimer(PlayerCheckTimerHandle);
	}
}
