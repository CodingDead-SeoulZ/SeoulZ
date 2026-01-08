// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_RakeAttack.h"
#include "Monster/SZBossBanshee.h"
//#include "Monster/GA/SZGA_BansheeRakeAttack.h"
#include "GameMode/SZGameModeBase.h"
#include "Monster/SZLineAttackActor.h"

UAnimNotify_RakeAttack::UAnimNotify_RakeAttack()
{
}

FString UAnimNotify_RakeAttack::GetNotifyName_Implementation() const
{
	return TEXT("AttackSpawn");
}

void UAnimNotify_RakeAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ASZBossBanshee* Boss = Cast<ASZBossBanshee>(MeshComp->GetOwner()))
	{
		//USZGA_BansheeRakeAttack* Ability = Cast<USZGA_BansheeRakeAttack>(Boss->GetCurrentSkillAbility());
		//if (!Ability)
		//{
		//	UE_LOG(LogTemp, Error, TEXT("Can't find Ability!"));
		//	return;
		//}

		//UWorld* World = MeshComp->GetWorld(); // MeshComp 기준으로 World 가져오기
		//if (!World)
		//{
		//	UE_LOG(LogTemp, Log, TEXT("Can't find world!"));
		//	return;
		//}
	
	}
}
