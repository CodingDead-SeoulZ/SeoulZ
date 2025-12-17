// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/SZAT_Trace.h"

USZAT_Trace::USZAT_Trace()
{
}

USZAT_Trace* USZAT_Trace::CreateTask(UGameplayAbility* OwningAbility)
{
	USZAT_Trace* NewTask = NewAbilityTask<USZAT_Trace>(OwningAbility);

	return NewTask;
}

void USZAT_Trace::Activate()
{
	Super::Activate();

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast();
	}

	EndTask();
}

void USZAT_Trace::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
}
