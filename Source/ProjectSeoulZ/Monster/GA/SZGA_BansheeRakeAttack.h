// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/GA/SZGA_SpawnSkillBase.h"
#include "SZGA_BansheeRakeAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSEOULZ_API USZGA_BansheeRakeAttack : public USZGA_SpawnSkillBase
{
	GENERATED_BODY()

public:
	USZGA_BansheeRakeAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	//
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void SpawnActors(class USZPoolManager* PoolManager) override;


protected:
	UFUNCTION()
	void OnCompleteCallback();

	//
	UFUNCTION()
	void OnInterruptedCallback();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackMontage;

	float ElapsedTime = 0.f;
	float Duration = 1.f; // 1초 동안 늘리기
	FVector StartExtent;
	FVector TargetExtent;

	
	
};
