// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/SZNormalAIInterface.h"

// Add default functionality here for any ISZNormalAIInterface functions that are not pure virtual.
void ISZNormalAIInterface::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
}

void ISZNormalAIInterface::AttackByAI(class UBTTask_Attack* Task)
{
}

UAnimMontage* ISZNormalAIInterface::GetAttackAnimMontage()
{
	return nullptr;
}

int ISZNormalAIInterface::GetSectionCount()
{
	return 1;
}

void ISZNormalAIInterface::OnAttackHitNotify()
{
}
