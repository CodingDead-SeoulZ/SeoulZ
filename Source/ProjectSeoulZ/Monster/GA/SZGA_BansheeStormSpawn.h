// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/GA/SZGA_SpawnSkillBase.h"
#include "SZGA_BansheeStormSpawn.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSEOULZ_API USZGA_BansheeStormSpawn : public USZGA_SpawnSkillBase
{
	GENERATED_BODY()

public:
	USZGA_BansheeStormSpawn();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	//
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void SpawnActors(class USZPoolManager* PoolManager) override;

protected:
	UFUNCTION()
	void OnStormCompleteCallback();

	//
	UFUNCTION()
	void OnStormInterruptedCallback();

	
};
