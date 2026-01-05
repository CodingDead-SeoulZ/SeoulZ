// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_NormalAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interface/SZMonsterAttackInterface.h"

UAnimNotify_NormalAttackHitCheck::UAnimNotify_NormalAttackHitCheck()
{
}

FString UAnimNotify_NormalAttackHitCheck::GetNotifyName_Implementation() const
{
	// 애니메이션에서 표시되는 이름.
	return TEXT("GASAttackHitCheck");
}

void UAnimNotify_NormalAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// SZMonsterAttackInterface를 상속 받아 구현했다면 해당 몬스터가 구현한 OnAttackHitNotify()가 호출됨.
	if (AActor* Owner = MeshComp->GetOwner())
	{
		//
		if (ISZMonsterAttackInterface* HitFunc = Cast<ISZMonsterAttackInterface>(Owner))
		{
			HitFunc->OnAttackHitNotify();
		}
	}

}
