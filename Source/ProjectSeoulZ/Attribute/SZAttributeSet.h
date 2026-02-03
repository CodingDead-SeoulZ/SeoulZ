// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SZAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//---------------------------------------------------------------------------------------------------------
// Author       : 이혜성
// Date         : 2025-12-00
// Copyright    : CodingDead
//
// Description  : 플레이어와 몬스터에서 사용할 어트리뷰트 셋
//                
//                
//----------------------------------------------------------------------------------------------------------

UCLASS()
class PROJECTSEOULZ_API USZAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	USZAttributeSet();

	//
	ATTRIBUTE_ACCESSORS(USZAttributeSet, AttackRange);
	ATTRIBUTE_ACCESSORS(USZAttributeSet, MaxAttackRange);
	ATTRIBUTE_ACCESSORS(USZAttributeSet, AttackRadius);
	ATTRIBUTE_ACCESSORS(USZAttributeSet, MaxAttackRadius);
	ATTRIBUTE_ACCESSORS(USZAttributeSet, AttackRate);
	ATTRIBUTE_ACCESSORS(USZAttributeSet, MaxAttackRate);
	ATTRIBUTE_ACCESSORS(USZAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(USZAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(USZAttributeSet, AttackDamage);
	ATTRIBUTE_ACCESSORS(USZAttributeSet, MaxAttackDamage);

	//
	ATTRIBUTE_ACCESSORS(USZAttributeSet, Defense);

	// 어트리뷰트 셋에 있는 스탯을 활용하기 전에 호출되는 함수
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// 어트리뷰트 셋에 있는 스탯을 사용한 후에 호출되는 함수
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	void BeginPlay();

	void OnGEAppliedToSelf(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle);

protected:
	// 공격 사정거리
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta=(AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;

	// 최대 공격 사정거리
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;

	// 공격 범위
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;

	// 최대 공격 범위
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;

	// 공격 속도
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;

	// 최대 공격 속도
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;

	// 현재 체력
	UPROPERTY(BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// 최대 체력
	UPROPERTY(BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	// 공격 데미지
	UPROPERTY(BlueprintReadOnly, Category = "Attack",meta=(AllowPrivateAccess = true))
	FGameplayAttributeData AttackDamage;
	
	// 최대 공격 데미지
	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackDamage;

	// 방어력
	UPROPERTY(BlueprintReadOnly, Category = "Defense", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Defense;
};
