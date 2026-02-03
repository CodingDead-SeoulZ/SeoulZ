// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SpawnStorm.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSEOULZ_API UAnimNotify_SpawnStorm : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_SpawnStorm();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	//
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
