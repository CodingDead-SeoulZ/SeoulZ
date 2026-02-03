// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "BTTask_TurnToTarget.h"
#include "SZAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/SZNormalAIInterface.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	// 비헤이비어 트리에서 사용할 때 표시되는 노드의 이름.
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// AIController가 조종하고 있는 Pawn을 가져와서 검사함.
	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 블랙보드의 BBKEY_TARGET에 목표가 설정되어 있는지 확인.
	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (TargetPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 폰이 특정 인터페이스를 구현하고 있는지를 확인(몬스터의 데이터를 가져오는 인터페이스).
	ISZNormalAIInterface* AIPawn = Cast<ISZNormalAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 회전 속도 할당.
	float TurnSpeed = AIPawn->GetAITurnSpeed();
	
	//
	FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.0f;

	//
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	//
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(),TurnSpeed));

	return EBTNodeResult::Type();

}
