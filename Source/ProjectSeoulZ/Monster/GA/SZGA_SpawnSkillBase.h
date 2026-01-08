// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SZGA_SpawnSkillBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTSEOULZ_API USZGA_SpawnSkillBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void SpawnActors(class USZPoolManager* PoolManager);

protected:
	UPROPERTY(EditDefaultsOnly, Category = Spawn)
	TSubclassOf<AActor> SpawnClass;

	UPROPERTY(EditDefaultsOnly,Category = Spawn)
	int32 SpawnCount = 2;

	UPROPERTY(EditDefaultsOnly, Category = Spawn)
	float SpawnRadius = 700.f;

	UPROPERTY(EditDefaultsOnly, Category = Spawn)
	int MaxRetries =4;


// ����� ��Ÿ��
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> SkillMontage;
};
