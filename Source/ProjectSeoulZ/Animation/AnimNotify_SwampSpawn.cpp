// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_SwampSpawn.h"
#include "Monster/SZBossBanshee.h"
#include "Monster/GA/SZGA_BansheeSwampSpawn.h"
#include "GameMode/SZGameModeBase.h"

UAnimNotify_SwampSpawn::UAnimNotify_SwampSpawn()
{
}

FString UAnimNotify_SwampSpawn::GetNotifyName_Implementation() const
{
	return TEXT("SwampSpawn");
}

void UAnimNotify_SwampSpawn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (ASZBossBanshee* Boss = Cast<ASZBossBanshee>(MeshComp->GetOwner()))
	{
		USZGA_BansheeSwampSpawn* Ability = Cast<USZGA_BansheeSwampSpawn>(Boss->GetCurrentSkillAbility());
		if (!Ability)
		{
			UE_LOG(LogTemp, Error, TEXT("Can't find Ability!"));
			return;   
		}

		UWorld* World = MeshComp->GetWorld(); // MeshComp 기준으로 World 가져오기
		if (!World)
		{
			UE_LOG(LogTemp, Log, TEXT("Can't find world!"));
			return;
		}
		ASZGameModeBase* GameMode = Cast<ASZGameModeBase>(World->GetAuthGameMode());
		if (!GameMode)
		{
			UE_LOG(LogTemp, Log, TEXT("Can't find GameMode!"));
			return;
		}
		if (GameMode->GetPoolManager()) // PoolManager 존재 확인
		{
			UE_LOG(LogTemp, Error, TEXT("find PoolManager!"));
			Ability->SpawnActors(GameMode->GetPoolManager());
		}else
		{
			UE_LOG(LogTemp, Log, TEXT("Can't find GameMode!"));
		}
	}
	
}
