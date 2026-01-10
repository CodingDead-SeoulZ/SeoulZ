// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetMoveTargetLocation.h"
#include "BTTask_SetMoveTargetLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_SetMoveTargetLocation::UBTTask_SetMoveTargetLocation()
{

}

EBTNodeResult::Type UBTTask_SetMoveTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AIController를 가져옴.
    AAIController* AICon = OwnerComp.GetAIOwner();
    
    // AIController의 Pawn, 즉 보스를 가져옴.
    APawn* Boss = AICon->GetPawn();

    // Player를 BlackBoard에서 Target을 가져와서 할당함.
    AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));

    // 보스와 플레이어가 할당되지 않았으면 반환.
    if (!Boss || !Player)
        return EBTNodeResult::Failed;

    // 플레이어와 보스의 위치를 가져옴.
    FVector PlayerLoc = Player->GetActorLocation();
    FVector BossLoc = Boss->GetActorLocation();

    // 블랙보드에서 DisredDistance를 가져옴.
    float DesiredDistance =
        OwnerComp.GetBlackboardComponent()->GetValueAsFloat("DesiredDistance");

    FVector Direction = (BossLoc - PlayerLoc).GetSafeNormal();
    FVector MoveLoc = PlayerLoc + Direction * DesiredDistance;

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

    FNavLocation Projected;
    if (NavSys->ProjectPointToNavigation(MoveLoc, Projected))
    {
        MoveLoc = Projected.Location;
    }

    OwnerComp.GetBlackboardComponent()
        ->SetValueAsVector("MoveTargetLocation", MoveLoc);
    
    OwnerComp.GetBlackboardComponent()->SetValueAsBool("bHasChasedPlayer", true);

    return EBTNodeResult::Succeeded;
}
