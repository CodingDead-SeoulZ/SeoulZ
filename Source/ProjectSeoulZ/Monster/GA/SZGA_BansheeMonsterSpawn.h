// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/GA/SZGA_SpawnSkillBase.h"
#include "SZGA_BansheeMonsterSpawn.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSEOULZ_API USZGA_BansheeMonsterSpawn : public USZGA_SpawnSkillBase
{
	GENERATED_BODY()

public:
	USZGA_BansheeMonsterSpawn();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	//
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnCompleteCallback();

	//
	UFUNCTION()
	void OnInterruptedCallback();

	
};
