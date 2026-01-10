// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_UpdateDistanceToPlayer.h"
#include "BTService_UpdateDistanceToPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateDistanceToPlayer::UBTService_UpdateDistanceToPlayer()
{
	NodeName = TEXT("UpdateDistance");

	// Tick�� ���� ����
	Interval = 1.0f;
}

void UBTService_UpdateDistanceToPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* AICon = OwnerComp.GetAIOwner();
    APawn* Pawn = AICon->GetPawn();

    AActor* Target =Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));

    if (!Pawn || !Target)
        return;

    float Distance = FVector::Dist(
        Pawn->GetActorLocation(),
        Target->GetActorLocation());

    OwnerComp.GetBlackboardComponent()
        ->SetValueAsFloat("DistanceToPlayer", Distance);

    if (Distance > 700.f && !OwnerComp.GetBlackboardComponent()->GetValueAsBool("bHasChasedPlayer"))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsBool("bCanMoveToPlayer", true);
    }
    else
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsBool("bCanMoveToPlayer", false);
    }

}
