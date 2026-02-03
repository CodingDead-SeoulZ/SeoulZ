// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/SZGameModeBase.h"
#include "SZGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/SZPoolManager.h"
#include "Monster/SZBossBanshee.h"
#include "Player/SZPlayerController.h"
#include "EngineUtils.h"
#include "SZGameInstance.h"
#include "UI/SZTimeLimitWidget.h"
#include "GameMode/SZGameInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

void ASZGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    USZGameInstance* GI = Cast<USZGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GI && GI->CurrentPhase == ESZPhase::Phase2)
    {
        StartPhase2();
    }
    else
    {
        StartPhase1();
    }

    PoolManager = GetGameInstance()->GetSubsystem<USZPoolManager>();

    // 스포너를 담을 배열. 특정 클래스가 들어가지 않기 때문에 Actor로 선언.
    TArray<AActor*> FoundActors;

    // 월드에 있는 SZMonsterSpawner를 FoundActors에 넣음.
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        ASZMonsterSpawner::StaticClass(),
        FoundActors
    );

    // Spawner들을 캐스팅해서 넣을 변수 변경.
    Spawners.Empty();

    // 캐스팅해서 Spawners에 저장
    for (AActor* Actor : FoundActors)
    {
        if (ASZMonsterSpawner* Spawner = Cast<ASZMonsterSpawner>(Actor))
        {
            Spawners.Add(Spawner);
        }
    }
    
    // 몬스터 스폰.
    SpawnStageMonsters();

    UE_LOG(LogTemp, Log, TEXT("GameMode BossHpBar"));
    ASZBossBanshee* Boss = nullptr;
    for (TActorIterator<ASZBossBanshee> It(GetWorld()); It; ++It)
    {
        Boss = *It;
        break;
    }

    if (Boss)
    {
        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            if (ASZPlayerController* SZPC = Cast<ASZPlayerController>(PC))
            {
                
                SZPC->ShowBossHP(Boss);
            }
        }
    }
}

void ASZGameModeBase::SpawnStageMonsters()
{
    // 스폰해서 생길 몬스터(살아날) 몬스터의 수를 위한 변수
    AliveMonsterCount = 0;

    // Spawners의 변수 수만큼 반복
    for (ASZMonsterSpawner* Spawner : Spawners)
    {
        // Spawner가 nullptr이면 종료.
        if (!Spawner) continue;
 
        // PoolManager를 매개변수로 받아 Spawner의 SpawnAll 호출.
        Spawner->SpawnAll(PoolManager);
        // 생성된 수만 큼 AliveMonsterCount 변수에 더해줌.
        AliveMonsterCount += Spawner->GetSpawnCount();
    }

    UE_LOG(LogTemp, Log, TEXT("Stage Start - Alive Monsters: %d"), AliveMonsterCount);
}

void ASZGameModeBase::StartPhase1()
{
    UnFreezeAllPlayers();

    UWorld* world = GetWorld();
    if (!world) return;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!PlayerController) return;

    ASZTimeLimitWidget* HUD = Cast<ASZTimeLimitWidget>(PlayerController->GetHUD());
    if (!HUD) return;

    USZGameInstance* GameInstance = Cast<USZGameInstance>(UGameplayStatics::GetGameInstance(this));

    const float Normal = GameInstance ? GameInstance->Phase1LimitSeconds : 300.0f;
    const float Warning = GameInstance ? GameInstance->WarningSeconds : 60.0f;

    HUD->StartTimer(Normal, Warning);
}

void ASZGameModeBase::OnPhase1TimeExpired()
{
    // 시간 종료 시 플레이어 정지
    FreezeAllPlayers();

    if (ASZPlayerController* PlayerController = Cast<ASZPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
    {
        PlayerController->ShowMissionFailMenu();
    }
}

void ASZGameModeBase::FreezeAllPlayers()
{
    UWorld* World = GetWorld();
    if (!World) return;

    for (FConstPlayerControllerIterator PCI = World->GetPlayerControllerIterator(); PCI; ++PCI)
    {
        APlayerController* PC = PCI->Get();
        if (!PC) return;

        // 입력 차단
        PC->SetIgnoreMoveInput(true);
        PC->SetIgnoreLookInput(true);

        // 캐릭터 이동 정지
        if (ACharacter* Ch = Cast<ACharacter>(PC->GetPawn()))
        {
            if (UCharacterMovementComponent* MoveComp = Ch->GetCharacterMovement())
            {
                MoveComp->StopMovementImmediately();
                MoveComp->DisableMovement();
            }
            
            // 물리 움직임 방지
            Ch->GetCharacterMovement()->Velocity = FVector::ZeroVector;
        }
        else if (APawn* Pawn = PC->GetPawn())
        {
            // 캐릭터 외 Pawn 처리
            Pawn->DisableInput(PC);
        }
    }
}

void ASZGameModeBase::UnFreezeAllPlayers()
{
    UWorld* World = GetWorld();
    if (!World) return;

    for (FConstPlayerControllerIterator PCI = World->GetPlayerControllerIterator(); PCI; ++PCI)
    {
        APlayerController* PC = PCI->Get();
        if (!PC) continue;

        // 입력 차단 해제
        PC->SetIgnoreMoveInput(false);
        PC->SetIgnoreLookInput(false);

        // 캐릭터 이동 정지
        if (ACharacter* Ch = Cast<ACharacter>(PC->GetPawn()))
        {
            if (UCharacterMovementComponent* MoveComp = Ch->GetCharacterMovement())
            {
                MoveComp->StopMovementImmediately();
                MoveComp->SetMovementMode(MOVE_Walking);
            }
        }
    }
}

void ASZGameModeBase::StartPhase2()
{
    UnFreezeAllPlayers();

    UWorld* world = GetWorld();
    if (!world) return;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!PlayerController) return;

    // 사용자 코드가 이렇게 되어 있으니 동일하게 유지
    ASZTimeLimitWidget* HUD = Cast<ASZTimeLimitWidget>(PlayerController->GetHUD());
    if (!HUD) return;

    USZGameInstance* GameInstance = Cast<USZGameInstance>(UGameplayStatics::GetGameInstance(this));

    const float Normal = GameInstance ? GameInstance->Phase2LimitSeconds : 420.0f;
    const float Warning = GameInstance ? GameInstance->WarningSeconds : 60.0f;

    HUD->StartTimer(Normal, Warning);
}

void ASZGameModeBase::OnPhase2TimeExpired()
{
    // 시간 종료 시 플레이어 정지
    FreezeAllPlayers();

    if (ASZPlayerController* PlayerController = Cast<ASZPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
    {
        PlayerController->ShowMissionFailMenu();
    }
}

void ASZGameModeBase::HandleBossPhaseTrigger()
{
    if (!HasAuthority()) return;

    USZGameInstance* GI = Cast<USZGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GI)
    {
        GI->CurrentPhase = ESZPhase::Phase2;
    }

    UGameplayStatics::OpenLevel(this, FName("PowerPlant"));
}
