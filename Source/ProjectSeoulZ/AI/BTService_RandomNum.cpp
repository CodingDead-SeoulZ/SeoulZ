// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_RandomNum.h"
#include "BTService_RandomNum.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_RandomNum::UBTService_RandomNum()
{
	NodeName = TEXT("RandomNum");

	// Tick�� ���� ����
	Interval = 1.0f;
}

void UBTService_RandomNum::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Owner�� ���� �����ϴ��� �˻�
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return;
	}

	// ��(����)�� ��ġ
	FVector Center = ControllingPawn->GetActorLocation();
	// ��(����)�� ���� ����
	UWorld* World = ControllingPawn->GetWorld();

	// ���尡 �����ϴ��� �˻�
	if (World == nullptr)
	{
		return;
	}

	float num = FMath::FRand();
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("RandomNum", num);
}
