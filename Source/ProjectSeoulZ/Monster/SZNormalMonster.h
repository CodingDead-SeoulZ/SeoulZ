// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Character/SZCharacterBase.h"
#include "Interface/SZNormalAIInterface.h"
#include "SZNormalMonsterAttackData.h"
#include "Interface/SZMonsterAttackInterface.h"
#include "Interface/SZPoolableInterface.h"
#include "GameplayEffectTypes.h"
#include "SZNormalMonster.generated.h"

//﻿---------------------------------------------------------------------------------------------------------
// Author       : 이혜성
// Date       : 2025-12-14
// Copyright   : 회사 또는 팀 이름
//
// Description : 페이즈 1에서 사용할 몬스터의 클래스.
//               SZCharacterBase를 상속받아 사용하고 메시와 애니메이션 블루프린트를 바꿔서 사용함.
//               
//                 
//----------------------------------------------------------------------------------------------------------

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMonsterDeathFinished, AActor*);


UCLASS()
class PROJECTSEOULZ_API ASZNormalMonster : public ASZCharacterBase, public ISZNormalAIInterface, public ISZMonsterAttackInterface, public ISZPoolableInterface
{
	GENERATED_BODY()
	
public:
	// 생성자
	ASZNormalMonster();

	// 현재의 공격 태스크를 가져오는 함수.
	FORCEINLINE UBTTask_Attack* GetAttackTask(){ return CurrentAttackTask; }

	// 현재 
	FORCEINLINE void SetCurrentAttackAbility(class UGameplayAbility* Ability) { CurrentAttackAbility = Ability; }

	// Hp가 0이 되면 호출될 SetDead() 함수.
	virtual void SetDead() override;

	//
	FOnMonsterDeathFinished OnDeathFinished;

	// ASC를 가져오는 함수.
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	//
	UFUNCTION()
	void ReturnMonster();

// AI Section
protected:
	// 상속받은 인터페이스에서 구현할 
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	//
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI(class UBTTask_Attack* Task) override;

	//
	FAICharacterAttackFinished OnAttackFinished;

	//
	virtual UAnimMontage* GetAttackAnimMontage() override;
	virtual int GetSectionCount() override;

	// ASC 초기화를 위해 사용.
	virtual void PossessedBy(AController* NewController) override;

	//
	UPROPERTY()
	UBTTask_Attack* CurrentAttackTask;


// Attack Section
protected:
	//
	FORCEINLINE void ClearCurrentAttackAblility() { CurrentAttackAbility = nullptr; }

	//
	virtual void OnAttackHitNotify() override;

	//
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Attack")
	TObjectPtr<class USZNormalMonsterAttackData> AttackDataAsset;

	//
	UPROPERTY()
	class UGameplayAbility* CurrentAttackAbility;

// UI Section
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USZWidgetComponent> HpBar;

	// ObjectPool Section
protected:
	//
	virtual void OnSpawnFromPool_Implementation() override;

	//
	virtual void OnReturnToPool_Implementation() override;

	//
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> MonsterInitGE;

protected:
	virtual void Tick(float DeltaTime) override;


// Be Hit Section
public:

	void HitByPlayer(const FGameplayEffectContextHandle& EffectContext);

	void SetThePlayerForChase(const FGameplayEffectContextHandle& EffectContext);

// ASC Section
	void InitializeASC(AController* NewController);

// HitEffect Section
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* HitEffect;

};
