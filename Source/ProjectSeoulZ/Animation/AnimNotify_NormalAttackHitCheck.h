// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_NormalAttackHitCheck.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 이혜성
// Date         : 2025-12-00
// Copyright    : CodingDead
//
// Description  : 일반 몬스터가 공격할 때 애니메이션에 설정된 노티파이로 공격 판정을 한다. 
//                
//                
//----------------------------------------------------------------------------------------------------------

UCLASS()
class PROJECTSEOULZ_API UAnimNotify_NormalAttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	//
	UAnimNotify_NormalAttackHitCheck();

protected:
	//
	virtual FString GetNotifyName_Implementation() const override;
	//
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:

};
