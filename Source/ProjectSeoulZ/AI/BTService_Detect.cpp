// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "SZAI.h"
#include "AIController.h"
#include "Interface/SZNormalAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Physics/SZCollision.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UBTService_Detect::UBTService_Detect()
{
	// �����̺�� Ʈ�������� ��� �̸�.
	NodeName = TEXT("Detect");

	// Tick�� ���� ����
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

	// ������ �����͸� ��ȯ�ϴ� �������̽��� �ִ��� �˻�
	ISZNormalAIInterface* AIPawn = Cast<ISZNormalAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return;
	}

	// �÷��̾� ���� ���� 
	float DetectRadius = AIPawn->GetAIDetectRange();

	// ���� ���� �÷��̾ �ִ��� Ȯ���ϴ� ����. �� ������ �� ����.
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_SZACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	// �浹 ����� �� �÷��̾ ã�� �������� ������ �Ҵ�.
	if (bResult)
	{
		// �浹 ����� Ž��
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());

			//�浹�� ��ü�� �÷��̾�� ����
			if (Pawn && Pawn->IsPlayerControlled())
			{
				// �����忡 BBKEY_TARGET�� �÷��̾ ����.
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f , FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.2f);

				// �÷��̾ Ž���ϸ� ������ �ӷ� ����.
				if (ACharacter* MonsterCharacter = Cast<ACharacter>(ControllingPawn))
				{
					UCharacterMovementComponent* MoveComp = MonsterCharacter->GetCharacterMovement();
					if (MoveComp)
					{
						UE_LOG(LogTemp, Log, TEXT("Boss a"));
						MoveComp->MaxWalkSpeed = 300.f; // ��: �ӵ� ����
					}
				}

				return;
			}
		}	
	}

	// �÷��̾ ã�� ���ߴٸ� BBKEY_TARGET�� nullptr �Ҵ�
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}


