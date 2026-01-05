// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Character/SZCharacterBase.h"
#include "Interface/SZNormalAIInterface.h"
#include "SZBossBanshee.generated.h"

/**
 * 
 */
class UGameplayAbility;

UCLASS()
class PROJECTSEOULZ_API ASZBossBanshee : public ASZCharacterBase, public ISZNormalAIInterface
{
	GENERATED_BODY()

public:
	// ������
	ASZBossBanshee();

	virtual void BeginPlay() override;

	// �׾��� �� ȣ��Ǵ� �Լ�.
	virtual void SetDead() override;

	// ASC ��������
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE UGameplayAbility* GetCurrentSkillAbility() { return CurrentSkillAbility; }
	FORCEINLINE void SetCurrentSkillAbility(UGameplayAbility* Ability) { CurrentSkillAbility = Ability; }
	
	FORCEINLINE void SetCurrentTask(UBTTaskNode* Task) {CurrentTask = Task;}
	FORCEINLINE UBTTaskNode* GetCurrentTask() { return CurrentTask; }

	//
	UFUNCTION()
	void DestroyFunc();

// SZNormalInterface���� ��ӹ޾� ������ �Լ���
public:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

// SpawnSkill Section
public:
	FORCEINLINE UAnimMontage* GetSpawnSkillMontage() { return SpawnSkillMontage; }
	FORCEINLINE UAnimMontage* GetSpawnSwampMontage() { return SpawnSwampMontage; }

// GAS Ability ���� ����
	UPROPERTY()
	class UGameplayAbility* CurrentSkillAbility;


protected:
	virtual void PossessedBy(AController* NewController) override;

	void PlayIntroMontage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> RoarMontage;

	void FinishRoar();

// AttributeSet Section
protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> BossInitGE;

// SpawnSkill Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> SpawnSkillMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> SpawnSwampMontage;
	
	UPROPERTY()
	UBTTaskNode* CurrentTask;

// GAS Ability Section
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;
	
	
	
};
