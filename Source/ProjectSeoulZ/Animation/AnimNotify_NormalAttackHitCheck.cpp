// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_NormalAttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interface/SZMonsterAttackInterface.h"

UAnimNotify_NormalAttackHitCheck::UAnimNotify_NormalAttackHitCheck()
{
}

FString UAnimNotify_NormalAttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("GASAttackHitCheck");
}

void UAnimNotify_NormalAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (ISZMonsterAttackInterface* HitFunc = Cast<ISZMonsterAttackInterface>(Owner))
		{
			HitFunc->OnAttackHitNotify();
		}
	}

}
