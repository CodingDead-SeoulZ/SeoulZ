// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_SpawnStorm.h"
#include "Monster/SZBossBanshee.h"
#include "Monster/GA/SZGA_BansheeStormSpawn.h"
#include "GameMode/SZGameInstance.h"
#include "GameMode/SZPoolManager.h"

UAnimNotify_SpawnStorm::UAnimNotify_SpawnStorm()
{
}

FString UAnimNotify_SpawnStorm::GetNotifyName_Implementation() const
{
	return TEXT("StormSpawn");
}

void UAnimNotify_SpawnStorm::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ASZBossBanshee* Boss = Cast<ASZBossBanshee>(MeshComp->GetOwner()))
	{
		USZGA_BansheeStormSpawn* Ability = Cast<USZGA_BansheeStormSpawn>(Boss->GetCurrentSkillAbility());
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

		USZGameInstance* GameInstance = Cast<USZGameInstance>(Boss->GetGameInstance());
		if (GameInstance)
		{
			USZPoolManager* PoolManager = GameInstance->GetSubsystem<USZPoolManager>();
			if (PoolManager)
			{
				Ability->SpawnActors(PoolManager);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Can't find PoolManager!"));
			}
		}

	}
}
