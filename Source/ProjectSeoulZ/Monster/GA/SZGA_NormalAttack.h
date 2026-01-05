// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Interface/SZAttackAbilityInterface.h"
#include "SZGA_NormalAttack.generated.h"

//---------------------------------------------------------------------------------------------------------
// Author       : 
// Date         : 2025-12-00
// Copyright    : 
//
// Description  : 
//                
//                
//----------------------------------------------------------------------------------------------------------

UCLASS()
class PROJECTSEOULZ_API USZGA_NormalAttack : public UGameplayAbility, public ISZAttackAblilityInterface
{
	GENERATED_BODY()

public:
	USZGA_NormalAttack();

	//
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	//
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	//
	UFUNCTION()
	void OnCompleteCallback();

	//
	UFUNCTION()
	void OnInterruptedCallback();

	//
	virtual void HitCheck() override;
	
};
