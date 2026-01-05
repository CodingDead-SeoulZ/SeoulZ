// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/GA/SZGA_SpawnSkillBase.h"
#include "Utils/SpawnUtils.h"
#include "GameMode/SZPoolManager.h"
#include "Interface/SZPoolableInterface.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

void USZGA_SpawnSkillBase::SpawnActors(class USZPoolManager* PoolManager)
{
    UE_LOG(LogTemp,Log,TEXT("Spawning Actors in SkillMontage"));
    AActor* Owner = GetAvatarActorFromActorInfo();
    if (!Owner)
    {
        UE_LOG(LogTemp,Warning,TEXT("Actor info is null"));
        return;
    }
    
    UWorld* World = Owner->GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp,Warning,TEXT("World info is null"));
        return;
    }
        

    for (int32 i = 0; i < SpawnCount; ++i)
    {
        FVector SpawnLocation;
        bool bFound = false;

        for (int32 j = 0; j < MaxRetries; j++)
        {
            //
            if (!SpawnUtil::GetRandomSpawnLocation(GetWorld(), Owner->GetActorLocation(), SpawnRadius, SpawnLocation))
            {
                continue;
            }

            //
            if (SpawnUtil::IsLocationFree(GetWorld(), SpawnLocation))
            {
                bFound = true;
                break;
            }
        }

        if (!bFound)
        {
            UE_LOG(LogTemp, Log, TEXT("Failed to find valid spawn location"));
            return;
        }

        FHitResult HitResult;
        FVector TraceStart = SpawnLocation + FVector(0.f, 0.f, 200.f); // ����� ������
        FVector TraceEnd = SpawnLocation - FVector(0.f, 0.f, 100.f); // ����� �Ʒ�����

        //
        if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_WorldStatic))
        {
            UE_LOG(LogTemp, Warning, TEXT("Spawn HitResult"));
            SpawnLocation.Z = HitResult.Location.Z; // ���� ���̷� Z ����
        }

        //
        FTransform SpawnTransform;
        //
        SpawnTransform.SetLocation(SpawnLocation);

        //
        AActor* Actor = PoolManager->GetActor(SpawnClass, SpawnTransform);

        //
        if (ACharacter* Character = Cast<ACharacter>(Actor))
        {
            //
            if (UCapsuleComponent* Capsule = Character->GetCapsuleComponent())
            {
                //
                FVector AdjustedLocation = SpawnLocation;
                AdjustedLocation.Z += Capsule->GetUnscaledCapsuleHalfHeight(); // Capsule �ٴ� ���� ����
                Character->SetActorLocation(AdjustedLocation);
            }
        }else
        {
            UE_LOG(LogTemp,Warning,TEXT("Character Not Ok!"));
        }

        //
        if (Actor && Actor->GetClass()->ImplementsInterface(USZPoolableInterface::StaticClass()))
        {
            //
            UE_LOG(LogTemp, Warning, TEXT("Spawn Ok!"));
            //
            ISZPoolableInterface::Execute_OnSpawnFromPool(Actor);
        }else
        {
            UE_LOG(LogTemp, Warning, TEXT("Spawn Not Ok!"));
        }
    }
}
